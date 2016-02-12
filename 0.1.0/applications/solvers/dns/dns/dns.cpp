// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "kmesh.hpp"
#include "uo_process.hpp"
#include "fft.hpp"
#include "calc_ek.hpp"
#include "graph.hpp"
#include "piso_control.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh_no_clear.inc"
  pisoControl piso{mesh};
  #include "read_transport_properties.inc"
  #include "create_fields.inc"
  #include "read_turbulence_properties.inc"
  #include "init_continuity_errs.inc"

  Info<< nl << "Starting time loop" << endl;
  while (runTime.loop())
  {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    force.internalField() = ReImSum
    (
      fft::reverseTransform
      (
        K/(mag(K) + 1.0e-6) ^ forceGen.newField(), K.nn()
      )
    );
    #include "global_properties.inc"
    fvVectorMatrix UEqn
    {
      fvm::ddt(U)
      + fvm::div(phi, U)
      - fvm::laplacian(nu, U)
      ==
      force
    };
    solve(UEqn == -fvc::grad(p));
    // --- PISO loop
    while (piso.correct())
    {
      volScalarField rAU{1.0/UEqn.A()};
      surfaceScalarField rAUf{"rAUf", fvc::interpolate(rAU)};
      volVectorField HbyA{"HbyA", U};
      HbyA = rAU*UEqn.H();
      surfaceScalarField phiHbyA
      {
        "phiHbyA",
        (fvc::interpolate(HbyA) & mesh.Sf()) + rAUf*fvc::ddtCorr(U, phi)
      };
      fvScalarMatrix pEqn
      {
        fvm::laplacian(rAUf, p) == fvc::div(phiHbyA)
      };
      pEqn.solve(mesh.solver(p.select(piso.finalInnerIter())));
      phi = phiHbyA - pEqn.flux();
      #include "continuity_errs.inc"
      U = HbyA - rAU*fvc::grad(p);
      U.correctBoundaryConditions();
    }
    runTime.write();
    if (runTime.outputTime())
    {
      calcEk(U, K).write
      (
        runTime.path()/"graphs"/runTime.timeName(),
        "Ek",
        runTime.graphFormat()
      );
    }
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "piso_control.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pisoControl piso{mesh};
  #include "create_fields.inc"
  #include "init_continuity_errs.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    #include "courant_no.inc"
    // Momentum predictor
    fvVectorMatrix UEqn
    {
      fvm::ddt(U)
    + fvm::div(phi, U)
    - fvm::laplacian(nu, U)
    };
    if (piso.momentumPredictor())
      solve(UEqn == -fvc::grad(p));
    // --- PISO loop
    while (piso.correct()) {
      volScalarField rAU{1.0/UEqn.A()};
      volVectorField HbyA{"HbyA", U};
      HbyA = rAU*UEqn.H();
      surfaceScalarField phiHbyA
      {
        "phiHbyA",
        (fvc::interpolate(HbyA) & mesh.Sf())
          + fvc::interpolate(rAU)*fvc::ddtCorr(U, phi)
      };
      adjustPhi(phiHbyA, U, p);
      // Non-orthogonal pressure corrector loop
      while (piso.correctNonOrthogonal()) {
        // Pressure corrector
        fvScalarMatrix pEqn
        {
          fvm::laplacian(rAU, p)
        ==
          fvc::div(phiHbyA)
        };
        pEqn.setReference(pRefCell, pRefValue);
        pEqn.solve(mesh.solver(p.select(piso.finalInnerIter())));
        if (piso.finalNonOrthogonalIter())
          phi = phiHbyA - pEqn.flux();
      }
      #include "continuity_errs.inc"
      U = HbyA - rAU*fvc::grad(p);
      U.correctBoundaryConditions();
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

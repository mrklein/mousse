// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "pimple_control.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  pimpleControl pimple{mesh};
  #include "read_gravitational_acceleration.inc"
  #include "create_fields.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "\n Time = " << runTime.timeName() << nl << endl;
    #include "courant_no.inc"
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      surfaceScalarField phiv{"phiv", phi/fvc::interpolate(h)};
      fvVectorMatrix hUEqn
      {
        fvm::ddt(hU)
      + fvm::div(phiv, hU)
      };
      hUEqn.relax();
      if (pimple.momentumPredictor()) {
        if (rotating)
          solve(hUEqn + (F ^ hU) == -magg*h*fvc::grad(h + h0));
        else
          solve(hUEqn == -magg*h*fvc::grad(h + h0));
        // Constrain the momentum to be in the geometry if 3D geometry
        if (mesh.nGeometricD() == 3) {
          hU -= (gHat & hU)*gHat;
          hU.correctBoundaryConditions();
        }
      }
      // --- Pressure corrector loop
      while (pimple.correct()) {
        volScalarField rAU{1.0/hUEqn.A()};
        surfaceScalarField ghrAUf{magg*fvc::interpolate(h*rAU)};
        surfaceScalarField phih0{ghrAUf*mesh.magSf()*fvc::snGrad(h0)};
        volVectorField HbyA{"HbyA", hU};
        if (rotating)
          HbyA = rAU*(hUEqn.H() - (F ^ hU));
        else
          HbyA = rAU*hUEqn.H();
        surfaceScalarField phiHbyA
        {
          "phiHbyA",
          (fvc::interpolate(HbyA) & mesh.Sf())
          + fvc::interpolate(rAU)*fvc::ddtCorr(h, hU, phi)
          - phih0
        };
        while (pimple.correctNonOrthogonal()) {
          fvScalarMatrix hEqn
          {
            fvm::ddt(h)
          + fvc::div(phiHbyA)
          - fvm::laplacian(ghrAUf, h)
          };
          hEqn.solve(mesh.solver(h.select(pimple.finalInnerIter())));
          if (pimple.finalNonOrthogonalIter())
            phi = phiHbyA + hEqn.flux();
        }
        hU = HbyA - rAU*h*magg*fvc::grad(h + h0);
        // Constrain the momentum to be in the geometry if 3D geometry
        if (mesh.nGeometricD() == 3)
          hU -= (gHat & hU)*gHat;
        hU.correctBoundaryConditions();
      }
    }
    U == hU/h;
    hTotal == h + h0;
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

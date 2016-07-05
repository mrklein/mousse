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
  pisoControl bpiso{mesh, "BPISO"};
  #include "create_fields.inc"
  #include "init_continuity_errs.inc"

  Info << nl << "Starting time loop" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    #include "courant_no.inc"

    {
      fvVectorMatrix UEqn {
        fvm::ddt(U)
      + fvm::div(phi, U)
      - fvc::div(phiB, 2.0*DBU*B)
      - fvm::laplacian(nu, U)
      + fvc::grad(DBU*magSqr(B))
      };
      if (piso.momentumPredictor()) {
        solve(UEqn == -fvc::grad(p));
      }
      // --- PISO loop
      while (piso.correct()) {
        volScalarField rAU{1.0/UEqn.A()};
        surfaceScalarField rAUf{"rAUf", fvc::interpolate(rAU)};
        volVectorField HbyA{"HbyA", U};
        HbyA = rAU*UEqn.H();
        surfaceScalarField phiHbyA
        {
          "phiHbyA",
          (fvc::interpolate(HbyA) & mesh.Sf())
            + rAUf*fvc::ddtCorr(U, phi)
        };
        while (piso.correctNonOrthogonal()) {
          fvScalarMatrix pEqn
          {
            fvm::laplacian(rAUf, p) == fvc::div(phiHbyA)
          };
          pEqn.setReference(pRefCell, pRefValue);
          pEqn.solve(mesh.solver(p.select(piso.finalInnerIter())));
          if (piso.finalNonOrthogonalIter()) {
            phi = phiHbyA - pEqn.flux();
          }
        }
        #include "continuity_errs.inc"
        U = HbyA - rAU*fvc::grad(p);
        U.correctBoundaryConditions();
      }
    }
    // --- B-PISO loop
    while (bpiso.correct()) {
      fvVectorMatrix BEqn {
        fvm::ddt(B)
      + fvm::div(phi, B)
      - fvc::div(phiB, U)
      - fvm::laplacian(DB, B)
      };
      BEqn.solve();
      volScalarField rAB{1.0/BEqn.A()};
      surfaceScalarField rABf{"rABf", fvc::interpolate(rAB)};
      phiB = (fvc::interpolate(B) & mesh.Sf()) + rABf*fvc::ddtCorr(B, phiB);
      while (bpiso.correctNonOrthogonal()) {
        fvScalarMatrix pBEqn
        {
          fvm::laplacian(rABf, pB) == fvc::div(phiB)
        };
        pBEqn.solve(mesh.solver(pB.select(bpiso.finalInnerIter())));
        if (bpiso.finalNonOrthogonalIter()) {
          phiB -= pBEqn.flux();
        }
      }
      #include "magnetic_field_err.inc"
    }
    runTime.write();
  }
  Info << "End\n" << endl;
  return 0;
}


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
  #include "read_thermodynamic_properties.inc"
  #include "read_transport_properties.inc"
  #include "create_fields.inc"
  #include "init_continuity_errs.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    #include "compressible_courant_no.inc"
    solve(fvm::ddt(rho) + fvc::div(phi));
    // --- Pressure-velocity PIMPLE corrector loop
    while (pimple.loop()) {
      fvVectorMatrix UEqn {
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      - fvm::laplacian(mu, U)
      };
      solve(UEqn == -fvc::grad(p));
      // --- Pressure corrector loop
      while (pimple.correct()) {
        volScalarField rAU{"rAU", 1.0/UEqn.A()};
        surfaceScalarField rhorAUf
        {
          "rhorAUf",
          fvc::interpolate(rho*rAU)
        };
        U = rAU*UEqn.H();
        surfaceScalarField phid
        {
          "phid",
          psi*((fvc::interpolate(U) & mesh.Sf())
               + rhorAUf*fvc::ddtCorr(rho, U, phi)/fvc::interpolate(rho))
        };
        phi = (rhoO/psi)*phid;
        fvScalarMatrix pEqn {
          fvm::ddt(psi, p)
        + fvc::div(phi)
        + fvm::div(phid, p)
        - fvm::laplacian(rhorAUf, p)
        };
        pEqn.solve();
        phi += pEqn.flux();
        solve(fvm::ddt(rho) + fvc::div(phi));
        #include "compressible_continuity_errs.inc"
        U -= rAU*fvc::grad(p);
        U.correctBoundaryConditions();
      }
    }
    rho = rhoO + psi*p;
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

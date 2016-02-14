// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "psi_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fixed_rho_fv_patch_scalar_field.hpp"
#include "direction_interpolate.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "fvc_smooth.hpp"
int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"
  #include "create_time_controls.inc"
  #include "create_rdelta_t.inc"
  #include "read_flux_scheme.inc"
  dimensionedScalar v_zero{"v_zero", dimVolume/dimTime, 0.0};
  // Courant numbers used to adjust the time-step
  scalar CoNum = 0.0;
  scalar meanCoNum = 0.0;
  Info<< "\nStarting time loop\n" << endl;
  while (runTime.run())
  {
    // --- Directed interpolation of primitive fields onto faces
    surfaceScalarField rho_pos{interpolate(rho, pos)};
    surfaceScalarField rho_neg{interpolate(rho, neg)};
    surfaceVectorField rhoU_pos{interpolate(rhoU, pos, U.name())};
    surfaceVectorField rhoU_neg{interpolate(rhoU, neg, U.name())};
    volScalarField rPsi{"rPsi", 1.0/psi};
    surfaceScalarField rPsi_pos{interpolate(rPsi, pos, T.name())};
    surfaceScalarField rPsi_neg{interpolate(rPsi, neg, T.name())};
    surfaceScalarField e_pos{interpolate(e, pos, T.name())};
    surfaceScalarField e_neg{interpolate(e, neg, T.name())};
    surfaceVectorField U_pos{"U_pos", rhoU_pos/rho_pos};
    surfaceVectorField U_neg{"U_neg", rhoU_neg/rho_neg};
    surfaceScalarField p_pos{"p_pos", rho_pos*rPsi_pos};
    surfaceScalarField p_neg{"p_neg", rho_neg*rPsi_neg};
    surfaceScalarField phiv_pos{"phiv_pos", U_pos & mesh.Sf()};
    surfaceScalarField phiv_neg{"phiv_neg", U_neg & mesh.Sf()};
    volScalarField c{"c", sqrt(thermo.Cp()/thermo.Cv()*rPsi)};
    surfaceScalarField cSf_pos
    {
      "cSf_pos",
      interpolate(c, pos, T.name())*mesh.magSf()
    };
    surfaceScalarField cSf_neg
    {
      "cSf_neg",
      interpolate(c, neg, T.name())*mesh.magSf()
    };
    surfaceScalarField ap
    {
      "ap",
      max(max(phiv_pos + cSf_pos, phiv_neg + cSf_neg), v_zero)
    };
    surfaceScalarField am
    {
      "am",
      min(min(phiv_pos - cSf_pos, phiv_neg - cSf_neg), v_zero)
    };
    surfaceScalarField a_pos{"a_pos", ap/(ap - am)};
    surfaceScalarField amaxSf{"amaxSf", max(mag(am), mag(ap))};
    surfaceScalarField aSf{"aSf", am*a_pos};
    if (fluxScheme == "Tadmor")
    {
      aSf = -0.5*amaxSf;
      a_pos = 0.5;
    }
    surfaceScalarField a_neg{"a_neg", 1.0 - a_pos};
    phiv_pos *= a_pos;
    phiv_neg *= a_neg;
    surfaceScalarField aphiv_pos{"aphiv_pos", phiv_pos - aSf};
    surfaceScalarField aphiv_neg{"aphiv_neg", phiv_neg + aSf};
    // Reuse amaxSf for the maximum positive and negative fluxes
    // estimated by the central scheme
    amaxSf = max(mag(aphiv_pos), mag(aphiv_neg));
    #include "central_courant_no.inc"
    #include "read_time_controls.inc"
    if (LTS)
    {
      #include "set_rdelta_t.inc"
    }
    else
    {
      #include "set_delta_t.inc"
    }
    runTime++;
    Info<< "Time = " << runTime.timeName() << nl << endl;
    phi = aphiv_pos*rho_pos + aphiv_neg*rho_neg;
    surfaceVectorField phiUp
    {
      (aphiv_pos*rhoU_pos + aphiv_neg*rhoU_neg)
      + (a_pos*p_pos + a_neg*p_neg)*mesh.Sf()
    };
    surfaceScalarField phiEp
    {
      "phiEp",
      aphiv_pos*(rho_pos*(e_pos + 0.5*magSqr(U_pos)) + p_pos)
      + aphiv_neg*(rho_neg*(e_neg + 0.5*magSqr(U_neg)) + p_neg)
      + aSf*p_pos - aSf*p_neg
    };
    volScalarField muEff{"muEff", turbulence->muEff()};
    volTensorField tauMC{"tauMC", muEff*dev2(mousse::T(fvc::grad(U)))};
    // --- Solve density
    solve(fvm::ddt(rho) + fvc::div(phi));
    // --- Solve momentum
    solve(fvm::ddt(rhoU) + fvc::div(phiUp));
    U.dimensionedInternalField() =
      rhoU.dimensionedInternalField()/rho.dimensionedInternalField();
    U.correctBoundaryConditions();
    rhoU.boundaryField() == rho.boundaryField()*U.boundaryField();
    if (!inviscid)
    {
      solve
      (
        fvm::ddt(rho, U) - fvc::ddt(rho, U)
        - fvm::laplacian(muEff, U)
        - fvc::div(tauMC)
      );
      rhoU = rho*U;
    }
    // --- Solve energy
    surfaceScalarField sigmaDotU
    (
      "sigmaDotU",
      (fvc::interpolate(muEff)*mesh.magSf()*fvc::snGrad(U)
       + (mesh.Sf() & fvc::interpolate(tauMC)))
      & (a_pos*U_pos + a_neg*U_neg)
    );
    solve
    (
      fvm::ddt(rhoE)
      + fvc::div(phiEp)
      - fvc::div(sigmaDotU)
    );
    e = rhoE/rho - 0.5*magSqr(U);
    e.correctBoundaryConditions();
    thermo.correct();
    rhoE.boundaryField() ==
      rho.boundaryField()*
      (
        e.boundaryField() + 0.5*magSqr(U.boundaryField())
      );
    if (!inviscid)
    {
      solve
      (
        fvm::ddt(rho, e) - fvc::ddt(rho, e)
        - fvm::laplacian(turbulence->alphaEff(), e)
      );
      thermo.correct();
      rhoE = rho*(e + 0.5*magSqr(U));
    }
    p.dimensionedInternalField() =
      rho.dimensionedInternalField()/psi.dimensionedInternalField();
    p.correctBoundaryConditions();
    rho.boundaryField() == psi.boundaryField()*p.boundaryField();
    turbulence->correct();
    runTime.write();
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}

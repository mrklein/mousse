{
  // Standard face-flux compression coefficient
  surfaceScalarField phic{interface.cAlpha()*mag(phi/mesh.magSf())};
  // Add the optional isotropic compression contribution
  if (icAlpha > 0) {
    phic *= (1.0 - icAlpha);
    phic += (interface.cAlpha()*icAlpha)*fvc::interpolate(mag(U));
  }
  volScalarField divU{fvc::div(phi)};
  if (nAlphaSubCycles > 1) {
    dimensionedScalar totalDeltaT = runTime.deltaT();
    surfaceScalarField rhoPhiSum{"rhoPhiSum", rhoPhi};
    for (subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
         !(++alphaSubCycle).end();) {
      #include "alpha_eqn.inc"
      rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
    }
    rhoPhi = rhoPhiSum;
  } else {
    #include "alpha_eqn.inc"
  }
  rho == alpha1*rho1 + alpha2*rho2;
}


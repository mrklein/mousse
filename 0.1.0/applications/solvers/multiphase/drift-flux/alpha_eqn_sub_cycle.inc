{
  surfaceScalarField alphaPhi
  {
    {
      "alphaPhi",
      runTime.timeName(),
      mesh
    },
    mesh,
    {"0", phi.dimensions(), 0}
  };
  surfaceScalarField phir{mesh.Sf() & fvc::interpolate(UdmModel.Udm())};
  if (nAlphaSubCycles > 1) {
    dimensionedScalar totalDeltaT = runTime.deltaT();
    surfaceScalarField alphaPhiSum
    {
      {
        "alphaPhiSum",
        runTime.timeName(),
        mesh
      },
      mesh,
      {"0", phi.dimensions(), 0}
    };
    for (subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
         !(++alphaSubCycle).end();) {
      #include "alpha_eqn.inc"
      alphaPhiSum += (runTime.deltaT()/totalDeltaT)*alphaPhi;
    }
    alphaPhi = alphaPhiSum;
  } else {
    #include "alpha_eqn.inc"
  }
  // Apply the diffusion term separately to allow implicit solution
  // and boundedness of the explicit advection
  {
    fvScalarMatrix alpha1Eqn
    {
      fvm::ddt(alpha1) - fvc::ddt(alpha1)
    - fvm::laplacian(turbulence->nut(), alpha1)
    };
    alpha1Eqn.solve(mesh.solver("alpha1Diffusion"));
    alphaPhi += alpha1Eqn.flux();
    alpha2 = 1.0 - alpha1;
    Info << "Phase-1 volume fraction = "
      << alpha1.weightedAverage(mesh.Vsc()).value()
      << "  Min(" << alpha1.name() << ") = " << min(alpha1).value()
      << "  Max(" << alpha1.name() << ") = " << max(alpha1).value()
      << endl;
  }
  rhoPhi = alphaPhi*(rho1 - rho2) + phi*rho2;
  rho = mixture.rho();
}

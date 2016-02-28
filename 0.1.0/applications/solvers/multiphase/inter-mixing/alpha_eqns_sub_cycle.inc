if (nAlphaSubCycles > 1)
{
  dimensionedScalar totalDeltaT = runTime.deltaT();
  surfaceScalarField rhoPhiSum
  {
    {
      "rhoPhiSum",
      runTime.timeName(),
      mesh
    },
    mesh,
    {"0", rhoPhi.dimensions(), 0}
  };
  for
  (
    subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
    !(++alphaSubCycle).end();
  )
  {
    #include "alpha_eqns.inc"
    rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
  }
  rhoPhi = rhoPhiSum;
}
else
{
  #include "alpha_eqns.inc"
}
{
  volScalarField rhoNew(alpha1*rho1 + alpha2*rho2 + alpha3*rho3);
  //solve(fvm::ddt(rho) + fvc::div(rhoPhi));
  //Info<< "density error = "
  //    << max((mag(rho - rhoNew)/mag(rhoNew))().internalField()) << endl;
  rho == rhoNew;
}

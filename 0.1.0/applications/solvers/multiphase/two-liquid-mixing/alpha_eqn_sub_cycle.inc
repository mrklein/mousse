#include "alpha_controls.inc"
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
    subCycle<volScalarField> alphaSubCycle{alpha1, nAlphaSubCycles};
    !(++alphaSubCycle).end();
  )
  {
    #include "alpha_eqn.inc"
    rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
  }
  rhoPhi = rhoPhiSum;
}
else
{
  #include "alpha_eqn.inc"
}
rho == alpha1*rho1 + alpha2*rho2;

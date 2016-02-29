Info<< "Reading field D\n" << endl;
volVectorField D
{
  {
    "D",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
autoPtr<volScalarField> Tptr(NULL);
if (thermalStress)
{
  Info<< "Reading field T\n" << endl;
  Tptr.reset
  (
    new volScalarField
    {
      {
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
      },
      mesh
    }
  );
}
Info<< "Calculating stress field sigmaD\n" << endl;
volSymmTensorField sigmaD
{
  {
    "sigmaD",
    runTime.timeName(),
    mesh
  },
  mu*twoSymm(fvc::grad(D)) + lambda*(I*tr(fvc::grad(D)))
};
Info<< "Calculating explicit part of div(sigma) divSigmaExp\n" << endl;
volVectorField divSigmaExp
{
  {
    "divSigmaExp",
    runTime.timeName(),
    mesh
  },
  fvc::div(sigmaD)
};
if (compactNormalStress)
{
  divSigmaExp -= fvc::laplacian(2*mu + lambda, D, "laplacian(DD,D)");
}
else
{
  divSigmaExp -= fvc::div((2*mu + lambda)*fvc::grad(D), "div(sigmaD)");
}
mesh.setFluxRequired(D.name());

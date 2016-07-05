Info << "Reading displacement field D\n" << endl;
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
Info << "Reading/setting displacement correction field Dcorr\n" << endl;
volVectorField Dcorr
{
  IOobject
  {
    "Dcorr",
    runTime.timeName(),
    mesh
  },
  D
};
Dcorr *= 0.0;
Info << "Calculating stress field sigmaD\n" << endl;
volSymmTensorField sigmaD
{
  {
    "sigmaD",
    runTime.timeName(),
    mesh
  },
  mu*twoSymm(fvc::grad(D)) + (lambda*I)*tr(fvc::grad(D))
};
volTensorField sigmaExp
{
  {
    "sigmaExp",
    runTime.timeName(),
    mesh
  },
  (lambda - mu)*fvc::grad(Dcorr) + mu*fvc::grad(Dcorr)().T()
  + (lambda*I)*tr(fvc::grad(Dcorr))
};

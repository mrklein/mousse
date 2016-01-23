Info<< "Reading field U\n" << endl;
volVectorField U
{
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info<< "Creating face flux\n" << endl;
surfaceScalarField phi
{
  {
    "phi",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::NO_WRITE
  },
  mesh,
  {"zero", mesh.Sf().dimensions()*U.dimensions(), 0.0}
};
singlePhaseTransportModel laminarTransport(U, phi);
autoPtr<incompressible::turbulenceModel> turbulence
(
  incompressible::turbulenceModel::New(U, phi, laminarTransport)
);
dimensionedVector Ubar{"Ubar", dimVelocity, laminarTransport};
vector flowDirection = (Ubar/mag(Ubar)).value();
tensor flowMask = sqr(flowDirection);
dimensionedVector gradP
{
  "gradP",
  dimLength/sqr(dimTime),
  vector::zero
};

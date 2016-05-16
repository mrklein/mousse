// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading field U\n" << endl;
volVectorField U
{
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};

Info << "Calculating wall distance field" << endl;
const volScalarField& y = wallDist::New(mesh).y();
// Set the mean boundary-layer thickness
dimensionedScalar ybl{"ybl", dimLength, 0};
if (args.optionFound("ybl")) {
  // If the boundary-layer thickness is provided use it
  ybl.value() = args.optionRead<scalar>("ybl");
} else if (args.optionFound("Cbl")) {
  // Calculate boundary layer thickness as Cbl*mean distance to wall
  ybl.value() = gAverage(y)*args.optionRead<scalar>("Cbl");
}
Info << "\nCreating boundary-layer for U of thickness "
  << ybl.value() << " m" << nl << endl;
Info << "Creating mask field" << endl;
volScalarField mask
{
  {
    "mask",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::NO_WRITE
  },
  mesh,
  {"zero", dimless, 0.0},
  zeroGradientFvPatchScalarField::typeName
};


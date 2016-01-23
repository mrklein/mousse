// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef create_phia_hpp_
#define create_phia_hpp_
Info<< "Reading/calculating face flux field phia\n" << endl;
surfaceScalarField phia
(
  IOobject
  (
    "phia",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  ),
  linearInterpolate(Ua) & mesh.Sf()
);
#endif

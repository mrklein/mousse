// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef create_phiv_hpp_
#define create_phiv_hpp_
Info<< "Reading/calculating face flux field phiv\n" << endl;
surfaceScalarField phiv
(
  IOobject
  (
    "phiv",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  ),
  linearInterpolate(U) & mesh.Sf()
);
#endif

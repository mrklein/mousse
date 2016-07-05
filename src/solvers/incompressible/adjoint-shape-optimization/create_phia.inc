#ifndef create_phia_inc_
#define create_phia_inc_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading/calculating face flux field phia\n" << endl;
surfaceScalarField phia
{
  IOobject
  {
    "phia",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  linearInterpolate(Ua) & mesh.Sf()
};

#endif

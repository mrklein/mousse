#ifndef CREATE_PHIV_HPP_
#define CREATE_PHIV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading/calculating face flux field phiv\n" << endl;
surfaceScalarField phiv
{
  {
    "phiv",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  linearInterpolate(U) & mesh.Sf()
};

#endif

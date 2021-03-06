#ifndef COMPRESSIBLE_CREATE_PHI_HPP_
#define COMPRESSIBLE_CREATE_PHI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading/calculating face flux field phi\n" << endl;
surfaceScalarField phi
{
  {
    "phi",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  linearInterpolate(rho*U) & mesh.Sf()
};

#endif

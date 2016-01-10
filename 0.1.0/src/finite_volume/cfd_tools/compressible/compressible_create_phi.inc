// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef compressible_create_phi_hpp_
#define compressible_create_phi_hpp_

Info<< "Reading/calculating face flux field phi\n" << endl;
surfaceScalarField phi
{
  IOobject
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

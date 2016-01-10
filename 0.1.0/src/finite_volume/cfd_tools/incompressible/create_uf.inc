// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef create_uf_hpp_
#define create_uf_hpp_

Info<< "Reading/calculating face velocity Uf\n" << endl;
surfaceVectorField Uf
{
  IOobject
  {
    "Uf",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  fvc::interpolate(U)
};

#endif

#ifndef CREATE_UF_HPP_
#define CREATE_UF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading/calculating face velocity Uf\n" << endl;
surfaceVectorField Uf
{
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

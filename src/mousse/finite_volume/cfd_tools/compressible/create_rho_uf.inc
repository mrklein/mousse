#ifndef CREATE_RHO_UF_HPP_
#define CREATE_RHO_UF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << "Reading/calculating face velocity rhoUf\n" << endl;
surfaceVectorField rhoUf
{
  {
    "rhoUf",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  fvc::interpolate(rho*U)
};

#endif

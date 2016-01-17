// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef create_rho_uf_hpp_
#define create_rho_uf_hpp_

Info<< "Reading/calculating face velocity rhoUf\n" << endl;
surfaceVectorField rhoUf
{
  IOobject
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

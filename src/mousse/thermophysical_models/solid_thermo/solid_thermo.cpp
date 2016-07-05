// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_thermo.hpp"
#include "fv_mesh.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(solidThermo, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(solidThermo, fvMesh);
DEFINE_RUN_TIME_SELECTION_TABLE(solidThermo, dictionary);

}


// Constructors 
mousse::solidThermo::solidThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicThermo{mesh, phaseName},
  rho_
  {
    {
      phasePropertyName("thermo:rho"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    dimDensity
  }
{}


mousse::solidThermo::solidThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  basicThermo{mesh, dict, phaseName},
  rho_
  {
    {
      phasePropertyName("thermo:rho"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    dimDensity
  }
{}


// Selectors
mousse::autoPtr<mousse::solidThermo> mousse::solidThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<solidThermo>(mesh, phaseName);
}


mousse::autoPtr<mousse::solidThermo> mousse::solidThermo::New
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
{
  return basicThermo::New<solidThermo>(mesh, dict, phaseName);
}


// Destructor 
mousse::solidThermo::~solidThermo()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::solidThermo::rho() const
{
  return rho_;
}


mousse::tmp<mousse::scalarField> mousse::solidThermo::rho(const label patchi) const
{
  return rho_.boundaryField()[patchi];
}


mousse::volScalarField& mousse::solidThermo::rho()
{
  return rho_;
}


bool mousse::solidThermo::read()
{
  return regIOobject::read();
}


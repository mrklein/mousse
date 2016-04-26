// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_thermo.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(psiThermo, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(psiThermo, fvMesh);

}


// Constructors 
mousse::psiThermo::psiThermo(const fvMesh& mesh, const word& phaseName)
:
  fluidThermo{mesh, phaseName},
  psi_
  {
    {
      phasePropertyName("thermo:psi"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    {0, -2, 2, 0, 0}
  },
  mu_
  {
    {
      phasePropertyName("thermo:mu"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    {1, -1, -1, 0, 0}
  }
{}


// Selectors
mousse::autoPtr<mousse::psiThermo> mousse::psiThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<psiThermo>(mesh, phaseName);
}


// Destructor 
mousse::psiThermo::~psiThermo()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::psiThermo::rho() const
{
  return p_*psi_;
}


mousse::tmp<mousse::scalarField> mousse::psiThermo::rho(const label patchi) const
{
  return p_.boundaryField()[patchi]*psi_.boundaryField()[patchi];
}


const mousse::volScalarField& mousse::psiThermo::psi() const
{
  return psi_;
}


mousse::tmp<mousse::volScalarField> mousse::psiThermo::mu() const
{
  return mu_;
}


mousse::tmp<mousse::scalarField> mousse::psiThermo::mu(const label patchi) const
{
  return mu_.boundaryField()[patchi];
}


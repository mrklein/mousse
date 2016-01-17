// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_thermo.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(rhoThermo, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(rhoThermo, fvMesh);
}
// Constructors 
mousse::rhoThermo::rhoThermo(const fvMesh& mesh, const word& phaseName)
:
  fluidThermo(mesh, phaseName),
  rho_
  (
    IOobject
    (
      phasePropertyName("thermo:rho"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimDensity
  ),
  psi_
  (
    IOobject
    (
      phasePropertyName("thermo:psi"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, -2, 2, 0, 0)
  ),
  mu_
  (
    IOobject
    (
      phasePropertyName("thermo:mu"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(1, -1, -1, 0, 0)
  )
{}
mousse::rhoThermo::rhoThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  fluidThermo(mesh, dict, phaseName),
  rho_
  (
    IOobject
    (
      phasePropertyName("thermo:rho"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimDensity
  ),
  psi_
  (
    IOobject
    (
      phasePropertyName("thermo:psi"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(0, -2, 2, 0, 0)
  ),
  mu_
  (
    IOobject
    (
      phasePropertyName("thermo:mu"),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimensionSet(1, -1, -1, 0, 0)
  )
{}
// Selectors
mousse::autoPtr<mousse::rhoThermo> mousse::rhoThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<rhoThermo>(mesh, phaseName);
}
// Destructor 
mousse::rhoThermo::~rhoThermo()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::rhoThermo::rho() const
{
  return rho_;
}
mousse::tmp<mousse::scalarField> mousse::rhoThermo::rho(const label patchi) const
{
  return rho_.boundaryField()[patchi];
}
mousse::volScalarField& mousse::rhoThermo::rho()
{
  return rho_;
}
const mousse::volScalarField& mousse::rhoThermo::psi() const
{
  return psi_;
}
mousse::tmp<mousse::volScalarField> mousse::rhoThermo::mu() const
{
  return mu_;
}
mousse::tmp<mousse::scalarField> mousse::rhoThermo::mu(const label patchi) const
{
  return mu_.boundaryField()[patchi];
}

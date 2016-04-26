// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fluid_thermo.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(fluidThermo, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(fluidThermo, fvMesh);

}


// Constructors 
mousse::fluidThermo::fluidThermo(const fvMesh& mesh, const word& phaseName)
:
  basicThermo{mesh, phaseName}
{}


mousse::fluidThermo::fluidThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  basicThermo{mesh, dict, phaseName}
{}


// Selectors
mousse::autoPtr<mousse::fluidThermo> mousse::fluidThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<fluidThermo>(mesh, phaseName);
}


// Destructor 
mousse::fluidThermo::~fluidThermo()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::fluidThermo::nu() const
{
  return mu()/rho();
}


mousse::tmp<mousse::scalarField> mousse::fluidThermo::nu(const label patchi) const
{
  return mu(patchi)/rho(patchi);
}


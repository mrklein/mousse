// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tether_potential.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(tetherPotential, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(tetherPotential, dictionary);

}


// Constructors 
mousse::tetherPotential::tetherPotential
(
  const word& name,
  const dictionary& tetherPotentialProperties
)
:
  name_{name},
  tetherPotentialProperties_{tetherPotentialProperties}
{}


// Member Functions 
bool mousse::tetherPotential::read(const dictionary& tetherPotentialProperties)
{
  tetherPotentialProperties_ = tetherPotentialProperties;
  return true;
}


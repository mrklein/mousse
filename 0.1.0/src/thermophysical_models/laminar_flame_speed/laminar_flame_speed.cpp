// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laminar_flame_speed.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(laminarFlameSpeed, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(laminarFlameSpeed, dictionary);

}


// Constructors 
mousse::laminarFlameSpeed::laminarFlameSpeed
(
  const dictionary& dict,
  const psiuReactionThermo& ct
)
:
  psiuReactionThermo_{ct},
  fuel_{dict.lookup("fuel")},
  equivalenceRatio_{0}
{
  if (!psiuReactionThermo_.composition().contains("ft")) {
    equivalenceRatio_ =
      dimensionedScalar{dict.lookup("equivalenceRatio")}.value();
  }
}

// Destructor 
mousse::laminarFlameSpeed::~laminarFlameSpeed()
{}


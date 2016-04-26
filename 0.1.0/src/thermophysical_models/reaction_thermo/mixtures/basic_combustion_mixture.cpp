// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_combustion_mixture.hpp"


// Static Member Functions
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(basicCombustionMixture, 0);

}


// Constructors 
mousse::basicCombustionMixture::basicCombustionMixture
(
  const dictionary& thermoDict,
  const wordList& specieNames,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicSpecieMixture{thermoDict, specieNames, mesh, phaseName}
{}


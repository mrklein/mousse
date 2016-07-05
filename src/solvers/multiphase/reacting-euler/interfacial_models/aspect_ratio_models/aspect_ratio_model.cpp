// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "aspect_ratio_model.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(aspectRatioModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(aspectRatioModel, dictionary);
}
// Constructors 
mousse::aspectRatioModel::aspectRatioModel
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  pair_{pair}
{}
// Destructor 
mousse::aspectRatioModel::~aspectRatioModel()
{}

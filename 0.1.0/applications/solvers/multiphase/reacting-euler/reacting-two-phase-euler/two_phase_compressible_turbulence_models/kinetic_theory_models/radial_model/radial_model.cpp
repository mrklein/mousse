// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radial_model.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(radialModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(radialModel, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::radialModel::radialModel
(
  const dictionary& dict
)
:
  dict_{dict}
{}
// Destructor 
mousse::kineticTheoryModels::radialModel::~radialModel()
{}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conductivity_model.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(conductivityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(conductivityModel, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::conductivityModel::conductivityModel
(
  const dictionary& dict
)
:
  dict_{dict}
{}
// Destructor 
mousse::kineticTheoryModels::conductivityModel::~conductivityModel()
{}

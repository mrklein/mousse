// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "frictional_stress_model.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(frictionalStressModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(frictionalStressModel, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::frictionalStressModel::frictionalStressModel
(
  const dictionary& dict
)
:
  dict_{dict}
{}
// Destructor 
mousse::kineticTheoryModels::frictionalStressModel::~frictionalStressModel()
{}

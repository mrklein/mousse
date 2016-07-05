// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "granular_pressure_model.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(granularPressureModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(granularPressureModel, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::granularPressureModel::granularPressureModel
(
  const dictionary& dict
)
:
  dict_{dict}
{}
// Destructor 
mousse::kineticTheoryModels::granularPressureModel::~granularPressureModel()
{}

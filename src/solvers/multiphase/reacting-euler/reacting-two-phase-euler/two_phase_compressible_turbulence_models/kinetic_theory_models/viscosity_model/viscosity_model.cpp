// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "viscosity_model.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
DEFINE_TYPE_NAME_AND_DEBUG(viscosityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(viscosityModel, dictionary);
}
}
// Constructors 
mousse::kineticTheoryModels::viscosityModel::viscosityModel
(
  const dictionary& dict
)
:
  dict_{dict}
{}
// Destructor 
mousse::kineticTheoryModels::viscosityModel::~viscosityModel()
{}

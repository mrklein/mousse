// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diameter_model.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(diameterModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(diameterModel, dictionary);
}
// Constructors 
mousse::diameterModel::diameterModel
(
  const dictionary& dict,
  const phaseModel& phase
)
:
  dict_{dict},
  phase_{phase}
{}
// Destructor 
mousse::diameterModel::~diameterModel()
{}

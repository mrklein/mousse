// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "saturation_model.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(saturationModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(saturationModel, dictionary);
}
// Constructors 
mousse::saturationModel::saturationModel()
{}
// Destructor 
mousse::saturationModel::~saturationModel()
{}

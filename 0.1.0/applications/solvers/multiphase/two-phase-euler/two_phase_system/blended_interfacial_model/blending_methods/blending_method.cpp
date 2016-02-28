// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_method.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(blendingMethod, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(blendingMethod, dictionary);
}
// Constructors 
mousse::blendingMethod::blendingMethod
(
  const dictionary& /*dict*/
)
{}
// Destructor 
mousse::blendingMethod::~blendingMethod()
{}

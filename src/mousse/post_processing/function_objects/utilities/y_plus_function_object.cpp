// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "y_plus_function_object.hpp"


// Static Data Members
namespace mousse {

DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(yPlusFunctionObject, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  functionObject,
  yPlusFunctionObject,
  dictionary
);

}


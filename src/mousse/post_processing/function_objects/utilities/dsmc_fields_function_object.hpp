#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_DSMC_FIELDS_DSMC_FIELDS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_DSMC_FIELDS_DSMC_FIELDS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_fields.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<dsmcFields>
  dsmcFieldsFunctionObject;

}

#endif


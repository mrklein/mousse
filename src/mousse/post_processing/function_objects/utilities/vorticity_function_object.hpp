#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_VORTICITY_VORTICITY_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_VORTICITY_VORTICITY_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vorticity.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<vorticity> vorticityFunctionObject;

}

#endif


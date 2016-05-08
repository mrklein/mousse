#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_WALL_SHEAR_STRESS_WALL_SHEAR_STRESS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_WALL_SHEAR_STRESS_WALL_SHEAR_STRESS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_shear_stress.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<wallShearStress>
  wallShearStressFunctionObject;

}

#endif


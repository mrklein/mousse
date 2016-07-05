#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_WALL_SHEAR_STRESS_IO_WALL_SHEAR_STRESS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_WALL_SHEAR_STRESS_IO_WALL_SHEAR_STRESS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_shear_stress.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<wallShearStress> IOwallShearStress;

}

#endif


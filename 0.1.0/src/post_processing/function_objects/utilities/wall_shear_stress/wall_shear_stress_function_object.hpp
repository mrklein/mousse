// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   wall_shear_stress_function_object.cpp
#ifndef wall_shear_stress_function_object_hpp_
#define wall_shear_stress_function_object_hpp_
#include "wall_shear_stress.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<wallShearStress>
    wallShearStressFunctionObject;
}
#endif

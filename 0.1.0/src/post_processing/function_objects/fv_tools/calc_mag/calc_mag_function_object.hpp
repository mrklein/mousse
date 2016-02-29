#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_MAG_CALC_MAG_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_MAG_CALC_MAG_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   calc_mag_function_object.cpp
#include "calc_mag.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<calcMag> calcMagFunctionObject;
}
#endif

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   calc_mag_function_object.cpp
#ifndef calc_mag_function_object_hpp_
#define calc_mag_function_object_hpp_
#include "calc_mag.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<calcMag> calcMagFunctionObject;
}
#endif

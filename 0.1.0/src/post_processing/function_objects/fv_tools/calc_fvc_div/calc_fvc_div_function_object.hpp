// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   calc_fvc_div_function_object.cpp
#ifndef calc_fvc_div_function_object_hpp_
#define calc_fvc_div_function_object_hpp_
#include "calc_fvc_div.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<calcFvcDiv> calcFvcDivFunctionObject;
}
#endif

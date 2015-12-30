// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   sampled_sets_function_object.cpp
#ifndef sampled_sets_function_object_hpp_
#define sampled_sets_function_object_hpp_
#include "sampled_sets.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<sampledSets>
    sampledSetsFunctionObject;
}
#endif

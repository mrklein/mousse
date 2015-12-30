// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   remove_registered_object_function_object.cpp
#ifndef remove_registered_object_function_object_hpp_
#define remove_registered_object_function_object_hpp_
#include "remove_registered_object.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<removeRegisteredObject>
    removeRegisteredObjectFunctionObject;
}
#endif

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_registered_object_function_object.cpp
#ifndef write_registered_object_function_object_hpp_
#define write_registered_object_function_object_hpp_
#include "write_registered_object.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<writeRegisteredObject>
    writeRegisteredObjectFunctionObject;
}
#endif

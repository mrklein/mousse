// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   time_activated_file_update_function_object.cpp
#ifndef time_activated_file_update_function_object_hpp_
#define time_activated_file_update_function_object_hpp_
#include "time_activated_file_update.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<timeActivatedFileUpdate>
    timeActivatedFileUpdateFunctionObject;
}
#endif

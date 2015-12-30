// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_dictionary_function_object.cpp
#ifndef write_dictionary_function_object_hpp_
#define write_dictionary_function_object_hpp_
#include "write_dictionary.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<writeDictionary>
    writeDictionaryFunctionObject;
}
#endif

#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_DICTIONARY_WRITE_DICTIONARY_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_DICTIONARY_WRITE_DICTIONARY_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_dictionary_function_object.cpp
#include "write_dictionary.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<writeDictionary>
    writeDictionaryFunctionObject;
}
#endif

#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_DICTIONARY_IO_WRITE_DICTIONARY_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_DICTIONARY_IO_WRITE_DICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_dictionary.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<writeDictionary> IOwriteDictionary;
}
#endif

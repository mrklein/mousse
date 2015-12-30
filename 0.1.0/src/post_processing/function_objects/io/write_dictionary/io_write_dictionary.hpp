// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_write_dictionary_hpp_
#define io_write_dictionary_hpp_
#include "write_dictionary.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<writeDictionary> IOwriteDictionary;
}
#endif

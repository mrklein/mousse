// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   table_readers.cpp
#ifndef table_readers_hpp_
#define table_readers_hpp_
#include "table_reader.hpp"
#include "field_types.hpp"

// Only used internally
#define MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, dataType)          \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typeTableReader< dataType >, 0)

// Sometimes used externally
#define MAKE_TABLE_READERS_TYPE_NAME(typeTableReader)                         \
                                                                              \
  MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, scalar);                 \
  MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, vector);                 \
  MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, sphericalTensor);        \
  MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, symmTensor);             \
  MAKE_TYPE_TABLE_READERS_TYPE_NAME(typeTableReader, tensor)

// Define type info for single dataType template instantiation (eg, vector)
#define MAKE_TABLE_READER_TYPE(typeTableReader, dataType)                     \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typeTableReader< dataType >, 0);  \
  ADD_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE                                   \
  (                                                                           \
    tableReader, typeTableReader, dataType, dictionary                        \
  )

// Define type info for scalar, vector etc. instantiations
#define MAKE_TABLE_READERS(typeTableReader)                                   \
                                                                              \
  MAKE_TABLE_READER_TYPE(typeTableReader, scalar);                            \
  MAKE_TABLE_READER_TYPE(typeTableReader, vector);                            \
  MAKE_TABLE_READER_TYPE(typeTableReader, sphericalTensor);                   \
  MAKE_TABLE_READER_TYPE(typeTableReader, symmTensor);                        \
  MAKE_TABLE_READER_TYPE(typeTableReader, tensor)
#endif

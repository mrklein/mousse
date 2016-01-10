// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   writers.cpp
#ifndef writers_hpp_
#define writers_hpp_
#include "writer.hpp"
#include "field_types.hpp"

// Only used internally
#define MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, dataType)                 \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typeWriter< dataType >, 0)

// Sometimes used externally
#define MAKE_SET_WRITERS_TYPE_NAME(typeWriter)                                \
                                                                              \
  MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, scalar);                        \
  MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, vector);                        \
  MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, sphericalTensor);               \
  MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, symmTensor);                    \
  MAKE_TYPE_SET_WRITERS_TYPE_NAME(typeWriter, tensor)

// Define type info for single dataType template instantiation (eg, vector)
#define MAKE_SET_WRITER_TYPE(typeWriter, dataType)                            \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typeWriter< dataType >, 0);       \
  ADD_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE                                   \
  (                                                                           \
    writer, typeWriter, dataType, word                                        \
  )

// Define type info for scalar, vector etc. instantiations
#define MAKE_SET_WRITERS(typeWriter)                                          \
                                                                              \
  MAKE_SET_WRITER_TYPE(typeWriter, scalar);                                   \
  MAKE_SET_WRITER_TYPE(typeWriter, vector);                                   \
  MAKE_SET_WRITER_TYPE(typeWriter, sphericalTensor);                          \
  MAKE_SET_WRITER_TYPE(typeWriter, symmTensor);                               \
  MAKE_SET_WRITER_TYPE(typeWriter, tensor)
#endif

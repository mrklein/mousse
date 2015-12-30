// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   writers.cpp
#ifndef writers_hpp_
#define writers_hpp_
#include "writer.hpp"
#include "field_types.hpp"
// Only used internally
#define makeTypeSetWritersTypeName(typeWriter, dataType)                      \
                                       \
  defineNamedTemplateTypeNameAndDebug(typeWriter< dataType >, 0)
// Sometimes used externally
#define makeSetWritersTypeName(typeWriter)                                    \
                                       \
  makeTypeSetWritersTypeName(typeWriter, scalar);                           \
  makeTypeSetWritersTypeName(typeWriter, vector);                           \
  makeTypeSetWritersTypeName(typeWriter, sphericalTensor);                  \
  makeTypeSetWritersTypeName(typeWriter, symmTensor);                       \
  makeTypeSetWritersTypeName(typeWriter, tensor)
// Define type info for single dataType template instantiation (eg, vector)
#define makeSetWriterType(typeWriter, dataType)                               \
                                       \
  defineNamedTemplateTypeNameAndDebug(typeWriter< dataType >, 0);           \
  addTemplatedToRunTimeSelectionTable                                       \
  (                                                                         \
    writer, typeWriter, dataType, word                                    \
  )
// Define type info for scalar, vector etc. instantiations
#define makeSetWriters(typeWriter)                                            \
                                       \
  makeSetWriterType(typeWriter, scalar);                                    \
  makeSetWriterType(typeWriter, vector);                                    \
  makeSetWriterType(typeWriter, sphericalTensor);                           \
  makeSetWriterType(typeWriter, symmTensor);                                \
  makeSetWriterType(typeWriter, tensor)
#endif

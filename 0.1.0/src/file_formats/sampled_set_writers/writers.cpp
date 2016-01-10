// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "writers.hpp"
namespace mousse
{
// Static Data Members
#define DEFINE_SET_WRITER_TYPE(dataType)                                      \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(writer<dataType >, 0);            \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE(writer, word, dataType);

DEFINE_SET_WRITER_TYPE(scalar);
DEFINE_SET_WRITER_TYPE(vector);
DEFINE_SET_WRITER_TYPE(sphericalTensor);
DEFINE_SET_WRITER_TYPE(symmTensor);
DEFINE_SET_WRITER_TYPE(tensor);
}  // namespace mousse

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "table_readers.hpp"

namespace mousse
{
// Static Data Members
#define DEFINE_TABLE_READER_TYPE(dataType)                                    \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(tableReader<dataType >, 0);       \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE(tableReader, dictionary, dataType);

DEFINE_TABLE_READER_TYPE(scalar);
DEFINE_TABLE_READER_TYPE(vector);
DEFINE_TABLE_READER_TYPE(sphericalTensor);
DEFINE_TABLE_READER_TYPE(symmTensor);
DEFINE_TABLE_READER_TYPE(tensor);

}  // namespace mousse

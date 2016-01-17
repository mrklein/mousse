// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "average_io_field.hpp"
#include "field_types.hpp"

namespace mousse
{
// Static Data Members
typedef AverageIOField<scalar> scalarAverageIOField;
typedef AverageIOField<vector> vectorAverageIOField;
typedef AverageIOField<sphericalTensor> sphericalTensorAverageIOField;
typedef AverageIOField<symmTensor> symmTensorAverageIOField;
typedef AverageIOField<tensor> tensorAverageIOField;

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  scalarAverageIOField,
  "scalarAverageField",
  0
);

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  vectorAverageIOField,
  "vectorAverageField",
  0
);

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  sphericalTensorAverageIOField,
  "sphericalTensorAverageField",
  0
);

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  symmTensorAverageIOField,
  "symmTensorAverageField",
  0
);

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  tensorAverageIOField,
  "tensorAverageField",
  0
);

}  // namespace mousse

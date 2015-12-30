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
defineTemplateTypeNameAndDebugWithName
(
  scalarAverageIOField,
  "scalarAverageField",
  0
);
defineTemplateTypeNameAndDebugWithName
(
  vectorAverageIOField,
  "vectorAverageField",
  0
);
defineTemplateTypeNameAndDebugWithName
(
  sphericalTensorAverageIOField,
  "sphericalTensorAverageField",
  0
);
defineTemplateTypeNameAndDebugWithName
(
  symmTensorAverageIOField,
  "symmTensorAverageField",
  0
);
defineTemplateTypeNameAndDebugWithName
(
  tensorAverageIOField,
  "tensorAverageField",
  0
);
}  // namespace mousse

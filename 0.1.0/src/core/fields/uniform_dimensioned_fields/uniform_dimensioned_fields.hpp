// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   uniform_dimensioned_fields.cpp

#ifndef uniform_dimensioned_fields_hpp_
#define uniform_dimensioned_fields_hpp_

#include "uniform_dimensioned_field.hpp"
#include "field_types.hpp"

namespace mousse
{
typedef UniformDimensionedField<scalar> uniformDimensionedScalarField;
typedef UniformDimensionedField<vector> uniformDimensionedVectorField;
typedef UniformDimensionedField<sphericalTensor>
  uniformDimensionedSphericalTensorField;
typedef UniformDimensionedField<symmTensor> uniformDimensionedSymmTensorField;
typedef UniformDimensionedField<tensor> uniformDimensionedTensorField;
}  // namespace mousse

#endif

#ifndef CORE_FIELDS_UNIFORM_DIMENSIONED_FIELDS_UNIFORM_DIMENSIONED_FIELDS_HPP_
#define CORE_FIELDS_UNIFORM_DIMENSIONED_FIELDS_UNIFORM_DIMENSIONED_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_dimensioned_field.hpp"
#include "field_types.hpp"


namespace mousse {

typedef UniformDimensionedField<scalar> uniformDimensionedScalarField;
typedef UniformDimensionedField<vector> uniformDimensionedVectorField;
typedef UniformDimensionedField<sphericalTensor>
  uniformDimensionedSphericalTensorField;
typedef UniformDimensionedField<symmTensor> uniformDimensionedSymmTensorField;
typedef UniformDimensionedField<tensor> uniformDimensionedTensorField;

}  // namespace mousse

#endif

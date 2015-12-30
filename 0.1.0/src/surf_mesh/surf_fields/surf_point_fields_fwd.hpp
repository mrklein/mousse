// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef surf_point_fields_fwd_hpp_
#define surf_point_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
class surfPointGeoMesh;
template<class Type, class GeoMesh>
class DimensionedField;
typedef DimensionedField<label, surfPointGeoMesh>
  surfPointLabelField;
typedef DimensionedField<scalar, surfPointGeoMesh>
  surfPointScalarField;
typedef DimensionedField<vector, surfPointGeoMesh>
  surfPointVectorField;
typedef DimensionedField<sphericalTensor, surfPointGeoMesh>
  surfPointSphericalTensorField;
typedef DimensionedField<symmTensor, surfPointGeoMesh>
  surfPointSymmTensorField;
typedef DimensionedField<tensor, surfPointGeoMesh>
  surfPointTensorField;
}  // namespace mousse
#endif

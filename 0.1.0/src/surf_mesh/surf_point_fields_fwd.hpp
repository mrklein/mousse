#ifndef SURF_MESH_SURF_FIELDS_SURF_POINT_FIELDS_FWD_HPP_
#define SURF_MESH_SURF_FIELDS_SURF_POINT_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

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

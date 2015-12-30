// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef surf_fields_fwd_hpp_
#define surf_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
class surfGeoMesh;
template<class Type, class GeoMesh>
class DimensionedField;
typedef DimensionedField<label, surfGeoMesh>  surfLabelField;
typedef DimensionedField<scalar, surfGeoMesh> surfScalarField;
typedef DimensionedField<vector, surfGeoMesh> surfVectorField;
typedef DimensionedField<sphericalTensor, surfGeoMesh> surfSphericalTensorField;
typedef DimensionedField<symmTensor, surfGeoMesh> surfSymmTensorField;
typedef DimensionedField<tensor, surfGeoMesh> surfTensorField;
}  // namespace mousse
#endif

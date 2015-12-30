// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef tri_surface_fields_fwd_hpp_
#define tri_surface_fields_fwd_hpp_
#include "field_types.hpp"
#include "tri_surface.hpp"
namespace mousse
{
template<class Type, class GeoMesh>
class DimensionedField;
class triSurfaceGeoMesh;
typedef mousse::DimensionedField<label, triSurfaceGeoMesh>
  triSurfaceLabelField;
typedef mousse::DimensionedField<scalar, triSurfaceGeoMesh>
  triSurfaceScalarField;
typedef mousse::DimensionedField<vector, triSurfaceGeoMesh>
  triSurfaceVectorField;
typedef mousse::DimensionedField<sphericalTensor, triSurfaceGeoMesh>
  triSurfaceSphericalTensorField;
typedef mousse::DimensionedField<symmTensor, triSurfaceGeoMesh>
  triSurfaceSymmTensorField;
typedef mousse::DimensionedField<tensor, triSurfaceGeoMesh>
  triSurfaceTensorField;
class triSurfacePointGeoMesh;
typedef mousse::DimensionedField<label, triSurfacePointGeoMesh>
  triSurfacePointLabelField;
typedef mousse::DimensionedField<scalar, triSurfacePointGeoMesh>
  triSurfacePointScalarField;
typedef mousse::DimensionedField<vector, triSurfacePointGeoMesh>
  triSurfacePointVectorField;
typedef mousse::DimensionedField<sphericalTensor, triSurfacePointGeoMesh>
  triSurfacePointSphericalTensorField;
typedef mousse::DimensionedField<symmTensor, triSurfacePointGeoMesh>
  triSurfacePointSymmTensorField;
typedef mousse::DimensionedField<tensor, triSurfacePointGeoMesh>
  triSurfacePointTensorField;
}  // namespace mousse
#endif

#ifndef FINITE_VOLUME_FIELDS_SURFACE_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_SURFACE_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   surface_fields.cpp
#include "field_types.hpp"
namespace mousse
{
class surfaceMesh;
template<class Type>
class fvsPatchField;
template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;
typedef GeometricField<scalar, fvsPatchField, surfaceMesh> surfaceScalarField;
typedef GeometricField<vector, fvsPatchField, surfaceMesh> surfaceVectorField;
typedef GeometricField<sphericalTensor, fvsPatchField, surfaceMesh>
  surfaceSphericalTensorField;
typedef GeometricField<symmTensor, fvsPatchField, surfaceMesh>
  surfaceSymmTensorField;
typedef GeometricField<tensor, fvsPatchField, surfaceMesh> surfaceTensorField;
}  // namespace mousse
#endif

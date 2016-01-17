// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   point_fields.cpp
#ifndef point_fields_fwd_hpp_
#define point_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
class pointMesh;
template<class Type>
class pointPatchField;
template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;
typedef GeometricField<scalar, pointPatchField, pointMesh> pointScalarField;
typedef GeometricField<vector, pointPatchField, pointMesh> pointVectorField;
typedef GeometricField<sphericalTensor, pointPatchField, pointMesh>
  pointSphericalTensorField;
typedef GeometricField<symmTensor, pointPatchField, pointMesh>
  pointSymmTensorField;
typedef GeometricField<tensor, pointPatchField, pointMesh> pointTensorField;
}  // namespace mousse
#endif

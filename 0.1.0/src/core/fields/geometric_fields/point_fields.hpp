#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_POINT_FIELDS_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_POINT_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   point_fields.cpp
#include "geometric_fields.hpp"
#include "field_types.hpp"
#include "point_patch_fields.hpp"
#include "point_mesh.hpp"
namespace mousse
{
typedef GeometricField<scalar, pointPatchField, pointMesh> pointScalarField;
typedef GeometricField<vector, pointPatchField, pointMesh> pointVectorField;
typedef GeometricField<sphericalTensor, pointPatchField, pointMesh>
  pointSphericalTensorField;
typedef GeometricField<symmTensor, pointPatchField, pointMesh>
  pointSymmTensorField;
typedef GeometricField<tensor, pointPatchField, pointMesh> pointTensorField;
}  // namespace mousse
#endif

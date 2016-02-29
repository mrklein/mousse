#ifndef FINITE_VOLUME_FIELDS_SLICED_SURFACE_FIELDS_HPP_
#define FINITE_VOLUME_FIELDS_SLICED_SURFACE_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sliced_geometric_field.hpp"
#include "sliced_fvs_patch_field.hpp"
#include "surface_mesh.hpp"
#include "sliced_surface_fields_fwd.hpp"
namespace mousse
{
template<>
inline void
SlicedGeometricField<scalar, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}
template<>
inline void
SlicedGeometricField<vector, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}
template<>
inline void
SlicedGeometricField
<
  sphericalTensor,
  fvsPatchField,
  slicedFvsPatchField,
  surfaceMesh
>::
correctBoundaryConditions()
{}
template<>
inline void
SlicedGeometricField
<
  symmTensor,
  fvsPatchField,
  slicedFvsPatchField,
  surfaceMesh
>::
correctBoundaryConditions()
{}
template<>
inline void
SlicedGeometricField<tensor, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}
}  // namespace mousse
#endif

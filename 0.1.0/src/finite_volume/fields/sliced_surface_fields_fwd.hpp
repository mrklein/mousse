#ifndef FINITE_VOLUME_FIELDS_SLICED_SURFACE_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_SLICED_SURFACE_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

class surfaceMesh;
template<class Type>
class fvsPatchField;

template<class Type>
class slicedFvsPatchField;
template
<
  class Type,
  template<class> class PatchField,
  template<class> class SlicedPatchField,
  class GeoMesh
>
class SlicedGeometricField;

typedef
SlicedGeometricField<scalar, fvsPatchField, slicedFvsPatchField, surfaceMesh>
  slicedSurfaceScalarField;

typedef
SlicedGeometricField<vector, fvsPatchField, slicedFvsPatchField, surfaceMesh>
  slicedSurfaceVectorField;

typedef
SlicedGeometricField
<
  sphericalTensor,
  fvsPatchField,
  slicedFvsPatchField,
  surfaceMesh
>
slicedSurfaceSphericalTensorField;

typedef
SlicedGeometricField
<
  symmTensor,
  fvsPatchField,
  slicedFvsPatchField,
  surfaceMesh
>
slicedSurfaceSymmTensorField;

typedef
SlicedGeometricField<tensor, fvsPatchField, slicedFvsPatchField, surfaceMesh>
slicedSurfaceTensorField;

}  // namespace mousse
#endif

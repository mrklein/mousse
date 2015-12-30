// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef sliced_vol_fields_fwd_hpp_
#define sliced_vol_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
class volMesh;
template<class Type>
class fvPatchField;
template<class Type>
class slicedFvPatchField;
template
<
  class Type,
  template<class> class PatchField,
  template<class> class SlicedPatchField,
  class GeoMesh
>
class SlicedGeometricField;
typedef
SlicedGeometricField<scalar, fvPatchField, slicedFvPatchField, volMesh>
  slicedVolScalarField;
typedef
SlicedGeometricField<vector, fvPatchField, slicedFvPatchField, volMesh>
  slicedVolVectorField;
typedef
SlicedGeometricField<sphericalTensor, fvPatchField, slicedFvPatchField, volMesh>
  slicedVolSphericalTensorField;
typedef
SlicedGeometricField<symmTensor, fvPatchField, slicedFvPatchField, volMesh>
  slicedVolSymmTensorField;
typedef
SlicedGeometricField<tensor, fvPatchField, slicedFvPatchField, volMesh>
  slicedVolTensorField;
}  // namespace mousse
#endif

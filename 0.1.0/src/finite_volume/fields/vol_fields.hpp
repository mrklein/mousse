#ifndef FINITE_VOLUME_FIELDS_VOL_FIELDS_HPP_
#define FINITE_VOLUME_FIELDS_VOL_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   vol_fields.cpp
#include "geometric_fields.hpp"
#include "vol_mesh.hpp"
#include "fv_mesh.hpp"
#include "fv_patch_field.hpp"
#include "vol_fields_fwd.hpp"
#include "calculated_fv_patch_fields.hpp"
namespace mousse
{
template<>
tmp<GeometricField<scalar, fvPatchField, volMesh> >
GeometricField<scalar, fvPatchField, volMesh>::component
(
  const direction
) const;
template<>
void GeometricField<scalar, fvPatchField, volMesh>::replace
(
  const direction,
  const GeometricField<scalar, fvPatchField, volMesh>& sf
);
}  // namespace mousse
#endif

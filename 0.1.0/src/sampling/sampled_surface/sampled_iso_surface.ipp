// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_iso_surface.hpp"
#include "vol_fields_fwd.hpp"
#include "point_fields.hpp"
#include "vol_point_interpolation.hpp"


// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledIsoSurface::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  // Recreate geometry if time has changed
  updateGeometry();
  return tmp<Field<Type>>{new Field<Type>{vField, surface().meshCells()}};
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledIsoSurface::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  // Get fields to sample. Assume volPointInterpolation!
  const GeometricField<Type, fvPatchField, volMesh>& volFld =
    interpolator.psi();
  // Recreate geometry if time has changed
  updateGeometry();
  if (subMeshPtr_.valid()) {
    tmp<GeometricField<Type, fvPatchField, volMesh>> tvolSubFld =
      subMeshPtr_().interpolate(volFld);
    const GeometricField<Type, fvPatchField, volMesh>& volSubFld =
      tvolSubFld();
    tmp<GeometricField<Type, pointPatchField, pointMesh>> tpointSubFld =
      volPointInterpolation::New(volSubFld.mesh()).interpolate(volSubFld);
    // Sample.
    return
      surface().interpolate
      (
        average_ ? pointAverage(tpointSubFld())() : volSubFld,
        tpointSubFld()
      );
  } else {
    tmp<GeometricField<Type, pointPatchField, pointMesh>> tpointFld =
      volPointInterpolation::New(volFld.mesh()).interpolate(volFld);
    // Sample.
    return
      surface().interpolate
      (
        average_ ? pointAverage(tpointFld())() : volFld,
        tpointFld()
      );
  }
}


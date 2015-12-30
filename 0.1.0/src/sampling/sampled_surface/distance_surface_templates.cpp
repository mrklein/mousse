// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distance_surface.hpp"
#include "vol_fields_fwd.hpp"
#include "point_fields.hpp"
#include "vol_point_interpolation.hpp"
// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::distanceSurface::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  if (cell_)
  {
    return tmp<Field<Type> >
    (
      new Field<Type>(vField, isoSurfCellPtr_().meshCells())
    );
  }
  else
  {
    return tmp<Field<Type> >
    (
      new Field<Type>(vField, isoSurfPtr_().meshCells())
    );
  }
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::distanceSurface::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
  // Get fields to sample. Assume volPointInterpolation!
  const GeometricField<Type, fvPatchField, volMesh>& volFld =
    interpolator.psi();
  tmp<GeometricField<Type, pointPatchField, pointMesh> > pointFld
  (
    volPointInterpolation::New(fvm).interpolate(volFld)
  );
  // Sample.
  if (cell_)
  {
    return isoSurfCellPtr_().interpolate
    (
      (
        average_
       ? pointAverage(pointFld())()
       : volFld
      ),
      pointFld()
    );
  }
  else
  {
    return isoSurfPtr_().interpolate
    (
      (
        average_
       ? pointAverage(pointFld())()
       : volFld
      ),
      pointFld()
    );
  }
}

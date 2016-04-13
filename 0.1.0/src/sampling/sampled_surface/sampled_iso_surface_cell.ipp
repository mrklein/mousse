// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_iso_surface_cell.hpp"
#include "iso_surface.hpp"
#include "vol_fields_fwd.hpp"
#include "point_fields.hpp"
#include "vol_point_interpolation.hpp"


// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledIsoSurfaceCell::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  // Recreate geometry if time has changed
  updateGeometry();
  return tmp<Field<Type>>{new Field<Type>{vField, meshCells_}};
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledIsoSurfaceCell::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  // Recreate geometry if time has changed
  updateGeometry();
  // One value per point
  tmp<Field<Type>> tvalues{new Field<Type>{points().size()}};
  Field<Type>& values = tvalues();
  boolList pointDone{points().size(), false};
  FOR_ALL(faces(), cutFaceI) {
    const face& f = faces()[cutFaceI];
    FOR_ALL(f, faceVertI) {
      label pointI = f[faceVertI];
      if (!pointDone[pointI]) {
        values[pointI] =
          interpolator.interpolate(points()[pointI], meshCells_[cutFaceI]);
        pointDone[pointI] = true;
      }
    }
  }
  return tvalues;
}


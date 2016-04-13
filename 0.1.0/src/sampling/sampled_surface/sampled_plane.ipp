// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_plane.hpp"


// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledPlane::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  return tmp<Field<Type>>{new Field<Type>{vField, meshCells()}};
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledPlane::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
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
          interpolator.interpolate(points()[pointI], meshCells()[cutFaceI]);
        pointDone[pointI] = true;
      }
    }
  }
  return tvalues;
}


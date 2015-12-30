// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_patch.hpp"
// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::sampledPatch::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  // One value per face
  tmp<Field<Type> > tvalues(new Field<Type>(patchFaceLabels_.size()));
  Field<Type>& values = tvalues();
  forAll(patchFaceLabels_, i)
  {
    label patchI = patchIDs_[patchIndex_[i]];
    const Field<Type>& bField = vField.boundaryField()[patchI];
    values[i] = bField[patchFaceLabels_[i]];
  }
  return tvalues;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::sampledPatch::sampleField
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
) const
{
  // One value per face
  tmp<Field<Type> > tvalues(new Field<Type>(patchFaceLabels_.size()));
  Field<Type>& values = tvalues();
  forAll(patchFaceLabels_, i)
  {
    label patchI = patchIDs_[patchIndex_[i]];
    values[i] = sField.boundaryField()[patchI][patchFaceLabels_[i]];
  }
  return tvalues;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::sampledPatch::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  // One value per vertex
  tmp<Field<Type> > tvalues(new Field<Type>(points().size()));
  Field<Type>& values = tvalues();
  const labelList& own = mesh().faceOwner();
  boolList pointDone(points().size(), false);
  forAll(faces(), cutFaceI)
  {
    label patchI = patchIDs_[patchIndex_[cutFaceI]];
    const polyPatch& pp = mesh().boundaryMesh()[patchI];
    label patchFaceI = patchFaceLabels()[cutFaceI];
    const face& f = faces()[cutFaceI];
    forAll(f, faceVertI)
    {
      label pointI = f[faceVertI];
      if (!pointDone[pointI])
      {
        label faceI = patchFaceI + pp.start();
        label cellI = own[faceI];
        values[pointI] = interpolator.interpolate
        (
          points()[pointI],
          cellI,
          faceI
        );
        pointDone[pointI] = true;
      }
    }
  }
  return tvalues;
}

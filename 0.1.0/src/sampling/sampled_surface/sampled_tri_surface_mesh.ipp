// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_tri_surface_mesh.hpp"


// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledTriSurfaceMesh::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  // One value per face
  tmp<Field<Type>> tvalues{new Field<Type>{sampleElements_.size()}};
  Field<Type>& values = tvalues();
  if (sampleSource_ == cells || sampleSource_ == insideCells) {
    // Sample cells
    FOR_ALL(sampleElements_, triI) {
      values[triI] = vField[sampleElements_[triI]];
    }
  } else {
    // Sample boundary faces
    const polyBoundaryMesh& pbm = mesh().boundaryMesh();
    label nBnd = mesh().nFaces() - mesh().nInternalFaces();
    // Create flat boundary field
    Field<Type> bVals{nBnd, pTraits<Type>::zero};
    FOR_ALL(vField.boundaryField(), patchI) {
      label bFaceI = pbm[patchI].start() - mesh().nInternalFaces();
      SubList<Type>{bVals, vField.boundaryField()[patchI].size(), bFaceI}
        .assign(vField.boundaryField()[patchI]);
    }
    // Sample in flat boundary field
    FOR_ALL(sampleElements_, triI) {
      label faceI = sampleElements_[triI];
      values[triI] = bVals[faceI-mesh().nInternalFaces()];
    }
  }
  return tvalues;
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::sampledTriSurfaceMesh::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  // One value per vertex
  tmp<Field<Type>> tvalues{new Field<Type>{sampleElements_.size()}};
  Field<Type>& values = tvalues();
  if (sampleSource_ == cells || sampleSource_ == insideCells) {
    // Sample cells.
    FOR_ALL(sampleElements_, pointI) {
      values[pointI] =
        interpolator.interpolate(samplePoints_[pointI],
                                 sampleElements_[pointI]);
    }
  } else {
    // Sample boundary faces.
    FOR_ALL(samplePoints_, pointI) {
      label faceI = sampleElements_[pointI];
      values[pointI] =
        interpolator.interpolate(samplePoints_[pointI],
                                 mesh().faceOwner()[faceI], faceI);
    }
  }
  return tvalues;
}


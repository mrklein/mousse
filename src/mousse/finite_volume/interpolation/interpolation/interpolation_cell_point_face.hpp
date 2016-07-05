#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_POINT_FACE_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_POINT_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPointFace
// Description
//   mousse::interpolationCellPointFace

#include "interpolation.hpp"
#include "fvs_patch_field.hpp"


namespace mousse {

template<class Type>
class interpolationCellPointFace
:
  public interpolation<Type>
{
  // Private data
    //- Interpolated volfield
    const GeometricField<Type, pointPatchField, pointMesh> psip_;
    //- Linearly interpolated volfield
    const GeometricField<Type, fvsPatchField, surfaceMesh> psis_;
    bool findTet
    (
      const vector& position,
      const label nFace,
      vector tetPoints[],
      label tetLabelCandidate[],
      label tetPointLabels[],
      scalar phi[],
      scalar phiCandidate[],
      label& closestFace,
      scalar& minDistance
    ) const;
    bool findTriangle
    (
      const vector& position,
      const label nFace,
      label tetPointLabels[],
      scalar phi[]
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("cellPointFace");
  // Constructors
    //- Construct from components
    interpolationCellPointFace
    (
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  // Member Functions
    //- Interpolate field to the given point in the given cell
    Type interpolate
    (
      const vector& position,
      const label cellI,
      const label faceI = -1
    ) const;
};
}  // namespace mousse

#include "interpolation_cell_point_face.ipp"

#endif

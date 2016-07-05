// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_cell_patch_constrained.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Constructor
template<class Type>
interpolationCellPatchConstrained<Type>::interpolationCellPatchConstrained
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  interpolation<Type>{psi}
{}


// Member Functions 
template<class Type>
Type interpolationCellPatchConstrained<Type>::interpolate
(
  const vector& /*pt*/,
  const label cellI,
  const label faceI
) const
{
  if (faceI >= 0 && faceI >= this->psi_.mesh().nInternalFaces()) {
    // Use boundary value
    const polyBoundaryMesh& pbm = this->psi_.mesh().boundaryMesh();
    label patchI = pbm.patchID()[faceI-this->psi_.mesh().nInternalFaces()];
    label patchFaceI = pbm[patchI].whichFace(faceI);
    return this->psi_.boundaryField()[patchI][patchFaceI];
  } else {
    return this->psi_[cellI];
  }
}

}  // namespace mousse

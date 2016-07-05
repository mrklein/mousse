#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_PATCH_CONSTRAINED_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_PATCH_CONSTRAINED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPatchConstrained
// Description
//   Uses the cell value for any point in the cell apart from a boundary face
//   where it uses the boundary value directly.
//   Note: will not work on an empty patch.

#include "interpolation.hpp"


namespace mousse {

class fvMesh;


template<class Type>
class interpolationCellPatchConstrained
:
  public interpolation<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("cellPatchConstrained");
  // Constructors
    //- Construct from components
    interpolationCellPatchConstrained
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

#include "interpolation_cell_patch_constrained.ipp"

#endif

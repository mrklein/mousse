// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPatchConstrained
// Description
//   Uses the cell value for any point in the cell apart from a boundary face
//   where it uses the boundary value directly.
//   Note: will not work on an empty patch.
#ifndef interpolation_cell_patch_constrained_hpp_
#define interpolation_cell_patch_constrained_hpp_
#include "interpolation.hpp"
namespace mousse
{
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
#ifdef NoRepository
#   include "interpolation_cell_patch_constrained.cpp"
#endif
#endif

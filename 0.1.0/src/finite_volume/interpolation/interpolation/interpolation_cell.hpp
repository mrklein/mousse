// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCell
// Description
//   Uses the cell value for any point in the cell
#ifndef interpolation_cell_hpp_
#define interpolation_cell_hpp_
#include "interpolation.hpp"
namespace mousse
{
class fvMesh;
template<class Type>
class interpolationCell
:
  public interpolation<Type>
{
public:
  //- Runtime type information
  TypeName("cell");
  // Constructors
    //- Construct from components
    interpolationCell
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
#   include "interpolation_cell.cpp"
#endif
#endif

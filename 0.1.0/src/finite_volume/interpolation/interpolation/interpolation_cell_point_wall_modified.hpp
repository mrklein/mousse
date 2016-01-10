// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPointWallModified
// Description
//   Same as interpolationCellPoint, but if interpolating a wall face, uses
//   cell centre value instead
#ifndef interpolation_cell_point_wall_modified_hpp_
#define interpolation_cell_point_wall_modified_hpp_
#include "interpolation_cell_point.hpp"
#include "cell_point_weight_wall_modified.hpp"
namespace mousse
{
template<class Type>
class interpolationCellPointWallModified
:
  public interpolationCellPoint<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("cellPointWallModified");
  // Constructors
    //- Construct from components
    interpolationCellPointWallModified
    (
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  // Member Functions
    //- Interpolate field for the given cellPointWeight
    inline Type interpolate(const cellPointWeightWallModified& cpw) const;
    //- Interpolate field to the given point in the given cell
    inline Type interpolate
    (
      const vector& position,
      const label cellI,
      const label faceI = -1
    ) const;
    //- Interpolate field to the given point in the tetrahedron
    //  defined by the given indices.
    inline Type interpolate
    (
      const vector& position,
      const tetIndices& tetIs,
      const label faceI = -1
    ) const;
};
}  // namespace mousse
#include "interpolation_cell_point_wall_modified_i.hpp"
#ifdef NoRepository
#   include "interpolation_cell_point_wall_modified.cpp"
#endif
#endif

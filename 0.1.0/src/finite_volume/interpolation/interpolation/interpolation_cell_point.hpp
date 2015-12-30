// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPoint
// Description
//   Given cell centre values and point (vertex) values decompose into
//   tetrahedra and linear interpolate within them.
#ifndef interpolation_cell_point_hpp_
#define interpolation_cell_point_hpp_
#include "interpolation.hpp"
#include "cell_point_weight.hpp"
namespace mousse
{
template<class Type>
class interpolationCellPoint
:
  public interpolation<Type>
{
protected:
  // Protected data
    //- Interpolated volfield
    const GeometricField<Type, pointPatchField, pointMesh> psip_;
public:
  //- Runtime type information
  TypeName("cellPoint");
  // Constructors
    //- Construct from components
    interpolationCellPoint
    (
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  // Member Functions
    //- Interpolate field for the given cellPointWeight
    inline Type interpolate(const cellPointWeight& cpw) const;
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
#include "interpolation_cell_point_i.hpp"
#ifdef NoRepository
#   include "interpolation_cell_point.cpp"
#endif
#endif

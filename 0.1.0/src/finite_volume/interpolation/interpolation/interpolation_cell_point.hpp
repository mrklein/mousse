#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_POINT_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_CELL_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationCellPoint
// Description
//   Given cell centre values and point (vertex) values decompose into
//   tetrahedra and linear interpolate within them.
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
  TYPE_NAME("cellPoint");
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

// Member Functions 
template<class Type>
inline Type mousse::interpolationCellPoint<Type>::interpolate
(
  const cellPointWeight& cpw
) const
{
  const List<scalar>& weights = cpw.weights();
  const List<label>& faceVertices = cpw.faceVertices();
  Type t = this->psi_[cpw.cell()]*weights[0];
  t += psip_[faceVertices[0]]*weights[1];
  t += psip_[faceVertices[1]]*weights[2];
  t += psip_[faceVertices[2]]*weights[3];
  return t;
}
template<class Type>
inline Type mousse::interpolationCellPoint<Type>::interpolate
(
  const vector& position,
  const label cellI,
  const label faceI
) const
{
  return interpolate(cellPointWeight(this->pMesh_, position, cellI, faceI));
}
template<class Type>
inline Type mousse::interpolationCellPoint<Type>::interpolate
(
  const vector& position,
  const tetIndices& tetIs,
  const label faceI
) const
{
  // Assumes that the position is consistent with the supplied
  // tetIndices.  Does not pay attention to whether or not faceI is
  // supplied or not - the result will be essentially the same.
  // Performs a consistency check, however.
  if (faceI >= 0)
  {
    if (faceI != tetIs.face())
    {
      FATAL_ERROR_IN
      (
        "inline Type mousse::interpolationCellPoint<Type>::interpolate"
        "("
          "const vector& position, "
          "const tetIndices& tetIs, "
          "const label faceI"
        ") const"
      )
      << "specified face " << faceI << " inconsistent with the face "
      << "stored by tetIndices: " << tetIs.face()
      << exit(FatalError);
    }
  }
  List<scalar> weights;
  tetIs.tet(this->pMesh_).barycentric(position, weights);
  const faceList& pFaces = this->pMesh_.faces();
  const face& f = pFaces[tetIs.face()];
  // Order of weights is the same as that of the vertices of the tet, i.e.
  // cellCentre, faceBasePt, facePtA, facePtB.
  Type t = this->psi_[tetIs.cell()]*weights[0];
  t += psip_[f[tetIs.faceBasePt()]]*weights[1];
  t += psip_[f[tetIs.facePtA()]]*weights[2];
  t += psip_[f[tetIs.facePtB()]]*weights[3];
  return t;
}
#ifdef NoRepository
#   include "interpolation_cell_point.cpp"
#endif
#endif

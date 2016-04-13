#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_CELL_POINT_WEIGHT_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_CELL_POINT_WEIGHT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellPointWeight
// Description
//   mousse::cellPointWeight

#include "vector.hpp"
#include "list.hpp"


namespace mousse {

class polyMesh;


class cellPointWeight
{
protected:
  // Protected data
   //- Cell index
   const label cellI_;
   //- Weights applied to tet vertices
   List<scalar> weights_;
   //- Face vertex indices
   List<label> faceVertices_;
  // Protected Member Functions
    void findTetrahedron
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI
    );
    void findTriangle
    (
      const polyMesh& mesh,
      const vector& position,
      const label faceI
    );
public:
  //- Debug switch
  static int debug;
  //- Tolerance used in calculating barycentric co-ordinates
  //  (applied to normalised values)
  static scalar tol;
  // Constructors
    //- Construct from components
    cellPointWeight
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label faceI = -1
    );
  // Member Functions
    //- Cell index
    inline label cell() const
    {
      return cellI_;
    }
    //- Interpolation weights
    inline const List<scalar>& weights() const
    {
      return weights_;
    }
    //- Interpolation addressing for points on face
    inline const List<label>& faceVertices() const
    {
      return faceVertices_;
    }
};
}  // namespace mousse
#endif

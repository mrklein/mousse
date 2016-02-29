#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_CELL_POINT_WEIGHT_WALL_MODIFIED_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_CELL_POINT_WEIGHT_WALL_MODIFIED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellPointWeightWallModified
// Description
//   mousse::cellPointWeightWallModified
// SourceFiles
//   cell_point_weight_wall_modified.cpp
#include "cell_point_weight.hpp"
#include "wall_poly_patch.hpp"
#include "poly_mesh.hpp"
#include "poly_boundary_mesh.hpp"
namespace mousse
{
class polyMesh;
class cellPointWeightWallModified
:
  public cellPointWeight
{
public:
  // Constructors
    //- Construct from components
    cellPointWeightWallModified
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label faceI = -1
    );
};
}  // namespace mousse
#endif

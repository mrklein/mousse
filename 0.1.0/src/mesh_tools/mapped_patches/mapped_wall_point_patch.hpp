#ifndef MESH_TOOLS_MAPPED_PATCHES_MAPPED_POINT_PATCH_MAPPED_WALL_POINT_PATCH_HPP_
#define MESH_TOOLS_MAPPED_PATCHES_MAPPED_POINT_PATCH_MAPPED_WALL_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedWallPointPatch
// Description
//   mappedWallPointPatch patch.

#include "wall_point_patch.hpp"
#include "mapped_wall_poly_patch.hpp"


namespace mousse {

class mappedWallPointPatch
:
  public wallPointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(mappedWallPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    mappedWallPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      wallPointPatch{patch, bm}
    {}
};
}  // namespace mousse
#endif

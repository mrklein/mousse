#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_WALL_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_WALL_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPointPatch
// Description
//   mousse::wallPointPatch

#include "face_point_patch.hpp"
#include "wall_poly_patch.hpp"


namespace mousse {

class wallPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(wallPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    wallPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      facePointPatch{patch, bm}
    {}
};

}  // namespace mousse

#endif


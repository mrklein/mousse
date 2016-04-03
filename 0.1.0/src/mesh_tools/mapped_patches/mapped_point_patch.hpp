#ifndef MESH_TOOLS_MAPPED_PATCHES_MAPPED_POINT_PATCH_MAPPED_POINT_PATCH_HPP_
#define MESH_TOOLS_MAPPED_PATCHES_MAPPED_POINT_PATCH_MAPPED_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedPointPatch
// Description
//   mappedPointPatch patch.

#include "face_point_patch.hpp"
#include "mapped_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

class mappedPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(mappedPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    mappedPointPatch
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

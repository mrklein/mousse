#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_MAPPED_WALL_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_DERIVED_MAPPED_WALL_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedWallFvPatch
// Description
//   mousse::mappedWallFvPatch
// SourceFiles
//   mapped_wall_fv_patch.cpp
#include "wall_fv_patch.hpp"
#include "mapped_wall_poly_patch.hpp"
namespace mousse
{
class mappedWallFvPatch
:
  public wallFvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(mappedWallPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    mappedWallFvPatch
    (
      const polyPatch& patch,
      const fvBoundaryMesh& bm
    )
    :
      wallFvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

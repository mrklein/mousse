#ifndef MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_POINT_PATCH_REGION_COUPLED_POINT_PATCH_HPP_
#define MESH_TOOLS_REGION_COUPLED_PATCHES_REGION_COUPLED_POINT_PATCH_REGION_COUPLED_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledPointPatch
// Description
//   Cyclic AMI point patch - place holder only
// SourceFiles
//   region_coupled_point_patch.cpp
#include "face_point_patch.hpp"
#include "region_coupled_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
class regionCoupledPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(regionCoupledPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    regionCoupledPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    ):
      facePointPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

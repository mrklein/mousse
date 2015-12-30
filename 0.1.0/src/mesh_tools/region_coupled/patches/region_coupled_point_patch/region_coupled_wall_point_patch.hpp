// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledWallPointPatch
// Description
//   Cyclic AMI point patch - place holder only
// SourceFiles
//   region_coupled_wall_point_patch.cpp
#ifndef region_coupled_wall_point_patch_hpp_
#define region_coupled_wall_point_patch_hpp_
#include "face_point_patch.hpp"
#include "region_coupled_wall_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
class regionCoupledWallPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TypeName(regionCoupledWallPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    regionCoupledWallPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    ):
      facePointPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

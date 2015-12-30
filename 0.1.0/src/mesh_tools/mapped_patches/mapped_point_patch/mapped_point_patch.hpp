// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedPointPatch
// Description
//   mappedPointPatch patch.
// SourceFiles
//   mapped_point_patch.cpp
#ifndef mapped_point_patch_hpp_
#define mapped_point_patch_hpp_
#include "face_point_patch.hpp"
#include "mapped_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
class mappedPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TypeName(mappedPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    mappedPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      facePointPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

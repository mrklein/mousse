// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPointPatch
// Description
//   mousse::wallPointPatch
// SourceFiles
//   wall_point_patch.cpp

#ifndef wall_point_patch_hpp_
#define wall_point_patch_hpp_

#include "face_point_patch.hpp"
#include "wall_poly_patch.hpp"

namespace mousse
{
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

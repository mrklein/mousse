// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericPointPatch
// Description
//   Substitute for unknown patches. Used for postprocessing when only
//   basic polyPatch info is needed.
// SourceFiles
//   generic_point_patch.cpp

#ifndef generic_point_patch_hpp_
#define generic_point_patch_hpp_

#include "face_point_patch.hpp"
#include "generic_poly_patch.hpp"

namespace mousse
{
class genericPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(genericPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    genericPointPatch
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
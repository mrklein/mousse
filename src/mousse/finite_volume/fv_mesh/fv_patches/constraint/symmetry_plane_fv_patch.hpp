#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_SYMMETRY_PLANE_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_SYMMETRY_PLANE_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlaneFvPatch
// Description
//   Symmetry-plane patch.
// SourceFiles
//   symmetry_plane_fv_patch.cpp
#include "fv_patch.hpp"
#include "symmetry_plane_poly_patch.hpp"
namespace mousse
{
class symmetryPlaneFvPatch
:
  public fvPatch
{
  // Private data
    //- Local reference cast into the symmetryPlane patch
    const symmetryPlanePolyPatch& symmetryPlanePolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(symmetryPlanePolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    symmetryPlaneFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm),
      symmetryPlanePolyPatch_
      (
        refCast<const symmetryPlanePolyPatch>(patch)
      )
    {}
  // Member Functions
    //- Return symmetry plane normal
    const vector& n() const
    {
      return symmetryPlanePolyPatch_.n();
    }
};
}  // namespace mousse
#endif

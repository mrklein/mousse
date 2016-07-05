#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_CYCLIC_SLIP_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_CYCLIC_SLIP_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicSlipFvPatch
// Description
//   Cyclic-plane patch.
// SourceFiles
//   cyclic_slip_fv_patch.cpp
#include "cyclic_fv_patch.hpp"
#include "cyclic_slip_poly_patch.hpp"
namespace mousse
{
class cyclicSlipFvPatch
:
  public cyclicFvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(cyclicSlipPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    cyclicSlipFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      cyclicFvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

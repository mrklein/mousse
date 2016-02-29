#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_BASIC_GENERIC_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_BASIC_GENERIC_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericFvPatch
// Description
//   FV variant of the genericPolyPatch.
// SourceFiles
//   generic_fv_patch.cpp
#include "fv_patch.hpp"
#include "generic_poly_patch.hpp"
namespace mousse
{
class genericFvPatch
:
  public fvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(genericPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    genericFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

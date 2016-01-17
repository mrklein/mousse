// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonuniformTransformCyclicFvPatch
// Description
//   Cyclic-plane patch.
// SourceFiles
//   nonuniform_transform_cyclic_fv_patch.cpp
#ifndef nonuniform_transform_cyclic_fv_patch_hpp_
#define nonuniform_transform_cyclic_fv_patch_hpp_
#include "cyclic_fv_patch.hpp"
#include "nonuniform_transform_cyclic_poly_patch.hpp"
namespace mousse
{
class nonuniformTransformCyclicFvPatch
:
  public cyclicFvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(nonuniformTransformCyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    nonuniformTransformCyclicFvPatch
    (
      const polyPatch& patch,
      const fvBoundaryMesh& bm
    )
    :
      cyclicFvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif

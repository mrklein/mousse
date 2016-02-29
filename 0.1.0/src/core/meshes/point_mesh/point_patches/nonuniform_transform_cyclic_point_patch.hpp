#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_NONUNIFORM_TRANSFORM_CYCLIC_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_NONUNIFORM_TRANSFORM_CYCLIC_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonuniformTransformCyclicPointPatch
// Description
//   Cyclic patch with slip constraint
// SourceFiles
//   nonuniform_transform_cyclic_point_patch.cpp
#include "cyclic_point_patch.hpp"
#include "nonuniform_transform_cyclic_poly_patch.hpp"
namespace mousse
{
class nonuniformTransformCyclicPointPatch
:
  public cyclicPointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(nonuniformTransformCyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    nonuniformTransformCyclicPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      cyclicPointPatch(patch, bm)
    {}
  // Destructor
    virtual ~nonuniformTransformCyclicPointPatch()
    {}
  // Member Functions
    //- Return point unit normals.
    virtual const vectorField& pointNormals() const;
    //- Accumulate the effect of constraint direction of this patch
    virtual void applyConstraint
    (
      const label pointi,
      pointConstraint&
    ) const;
};
}  // namespace mousse
#endif

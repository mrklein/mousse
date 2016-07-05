#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_EMPTY_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_EMPTY_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyPointPatch
// Description
//   Empty plane patch.

#include "face_point_patch.hpp"
#include "empty_poly_patch.hpp"


namespace mousse {

class emptyPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(emptyPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    emptyPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      facePointPatch{patch, bm}
    {}
  // Member Functions
    //- Return the constraint type this pointPatch implements.
    virtual const word& constraintType() const
    {
      return type();
    }
    //- Accumulate the effect of constraint direction of this patch
    virtual void applyConstraint
    (
      const label pointi,
      pointConstraint&
    ) const;
};
}  // namespace mousse
#endif

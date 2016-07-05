#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_WEDGE_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_WEDGE_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgePointPatch
// Description
//   Wedge front and back plane patch.

#include "face_point_patch.hpp"
#include "wedge_poly_patch.hpp"


namespace mousse {

class wedgePointPatch
:
  public facePointPatch
{
  // Private data
    //- Local reference cast into the symmetryPlane patch
    const wedgePolyPatch& wedgePolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(wedgePolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    wedgePointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
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
    //- Return symmetry plane normal
    const vector& n() const
    {
      return wedgePolyPatch_.n();
    }
};
}  // namespace mousse
#endif

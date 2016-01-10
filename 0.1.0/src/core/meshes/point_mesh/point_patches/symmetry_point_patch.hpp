// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPointPatch
// Description
//   Symmetry patch for non-planar or multi-plane patches.
// SourceFiles
//   symmetry_point_patch.cpp
//   see_also
//   symmetry_plane_point_patch
#ifndef symmetry_point_patch_hpp_
#define symmetry_point_patch_hpp_
#include "face_point_patch.hpp"
#include "symmetry_poly_patch.hpp"
namespace mousse
{
class symmetryPointPatch
:
  public facePointPatch
{
public:
  //- Runtime type information
  TYPE_NAME(symmetryPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    symmetryPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    )
    :
      facePointPatch(patch, bm)
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

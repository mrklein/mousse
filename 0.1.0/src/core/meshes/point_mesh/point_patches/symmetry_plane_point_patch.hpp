// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlanePointPatch
// Description
//   Symmetry-plane patch.
// SourceFiles
//   symmetry_plane_point_patch.cpp
#ifndef symmetry_plane_point_patch_hpp_
#define symmetry_plane_point_patch_hpp_
#include "face_point_patch.hpp"
#include "symmetry_plane_poly_patch.hpp"
namespace mousse
{
class symmetryPlanePointPatch
:
  public facePointPatch
{
  // Private data
    //- Local reference cast into the symmetryPlane patch
    const symmetryPlanePolyPatch& symmetryPlanePolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(symmetryPlanePolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    symmetryPlanePointPatch
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
      return symmetryPlanePolyPatch_.n();
    }
};
}  // namespace mousse
#endif

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyPointPatch
// Description
//   Empty plane patch.
// SourceFiles
//   empty_point_patch.cpp
#ifndef empty_point_patch_hpp_
#define empty_point_patch_hpp_
#include "face_point_patch.hpp"
#include "empty_poly_patch.hpp"
namespace mousse
{
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

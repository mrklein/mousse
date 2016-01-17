// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgePointPatch
// Description
//   Wedge front and back plane patch.
// SourceFiles
//   wedge_point_patch.cpp
#ifndef wedge_point_patch_hpp_
#define wedge_point_patch_hpp_
#include "face_point_patch.hpp"
#include "wedge_poly_patch.hpp"
namespace mousse
{
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

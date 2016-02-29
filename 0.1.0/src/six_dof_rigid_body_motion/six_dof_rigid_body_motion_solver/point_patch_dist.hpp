#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SOLVER_POINT_PATCH_DIST_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SOLVER_POINT_PATCH_DIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchDist
// Description
//   Calculation of distance to nearest patch for all points
// SourceFiles
//   point_patch_dist.cpp
#include "point_fields.hpp"
namespace mousse
{
class pointMesh;
class pointPatchDist
:
  public pointScalarField
{
private:
  // Private Member Data
    //- Reference to pointField
    const pointField& points_;
    //- Set of patch IDs
    const labelHashSet patchIDs_;
    //- Number of unset points
    label nUnset_;
public:
  // Constructors
    //- Construct from mesh and set of patches
    pointPatchDist
    (
      const pointMesh& pMesh,
      const labelHashSet& patchIDs,
      const pointField& points
    );
    //- Disallow default bitwise copy construct
    pointPatchDist(const pointPatchDist&) = delete;
    //- Disallow default bitwise assignment
    pointPatchDist& operator=(const pointPatchDist&) = delete;
  //- Destructor
  virtual ~pointPatchDist();
  // Member Functions
    const pointScalarField& y() const
    {
      return *this;
    }
    label nUnset() const
    {
      return nUnset_;
    }
    //- Correct for mesh geom/topo changes
    void correct();
};
}  // namespace mousse
#endif

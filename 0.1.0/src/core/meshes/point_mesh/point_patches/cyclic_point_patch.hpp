#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_CYCLIC_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_CYCLIC_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicPointPatch
// Description
//   Cyclic patch for post-processing.
// SourceFiles
//   cyclic_point_patch.cpp
#include "coupled_face_point_patch.hpp"
#include "cyclic_poly_patch.hpp"
#include "point_boundary_mesh.hpp"
namespace mousse
{
class cyclicPointPatch
:
  public coupledFacePointPatch
{
  // Private data
    //- Local reference cast into the cyclic patch
    const cyclicPolyPatch& cyclicPolyPatch_;
  // Private Member Functions
    //- Disallow default construct as copy
    cyclicPointPatch(const cyclicPointPatch&);
    //- Disallow default assignment
    void operator=(const cyclicPointPatch&);
  // Demand driven private data
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&);
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers&, const pointField&);
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers&, const pointField&);
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&);
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&);
public:
  //- Runtime type information
  TYPE_NAME(cyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    cyclicPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
  //- Destructor
  virtual ~cyclicPointPatch();
  // Member Functions
    // Access
      //- Return the constraint type this pointPatch implements.
      virtual const word& constraintType() const
      {
        return type();
      }
      //- Return the underlying cyclicPolyPatch
      const cyclicPolyPatch& cyclicPatch() const
      {
        return cyclicPolyPatch_;
      }
      //- Return neighbour point patch
      const cyclicPointPatch& neighbPatch() const
      {
        label patchI = cyclicPolyPatch_.neighbPatchID();
        const pointPatch& pp = this->boundaryMesh()[patchI];
        return refCast<const cyclicPointPatch>(pp);
      }
      //- Are the cyclic planes parallel
      bool parallel() const
      {
        return cyclicPolyPatch_.parallel();
      }
      //- Return face transformation tensor
      const tensorField& forwardT() const
      {
        return cyclicPolyPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      const tensorField& reverseT() const
      {
        return cyclicPolyPatch_.reverseT();
      }
    // Access functions for demand driven data
      //- Return the set of pairs of points that require transformation
      //  and/or mapping. First index is on this patch, second on the
      //  neighbour patch.
      virtual const edgeList& transformPairs() const;
};
}  // namespace mousse
#endif

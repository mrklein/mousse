#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_COUPLED_FACE_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_COUPLED_FACE_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledFacePointPatch
// Description
//   coupled patch for post-processing.  Used as the base class for processor
//   and cyclic pointPatches

#include "coupled_point_patch.hpp"
#include "face_point_patch.hpp"
#include "coupled_poly_patch.hpp"


namespace mousse {

class pointBoundaryMesh;


class coupledFacePointPatch
:
  public facePointPatch,
  public coupledPointPatch
{
  // Private data
    const coupledPolyPatch& coupledPolyPatch_;
protected:
    // Construction of demand-driven data
      //- Calculate mesh points
      virtual void calcGeometry(PstreamBuffers&) = 0;
public:
  //- Runtime type information
  TYPE_NAME(coupledPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    coupledFacePointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
    //- Disallow default construct as copy
    coupledFacePointPatch(const coupledFacePointPatch&) = delete;
    //- Disallow default assignment
    void operator=(const coupledFacePointPatch&) = delete;
  //- Destructor
  virtual ~coupledFacePointPatch();
};

}  // namespace mousse

#endif


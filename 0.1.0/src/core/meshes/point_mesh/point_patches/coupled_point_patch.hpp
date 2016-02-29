#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_COUPLED_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_COUPLED_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledPointPatch
// Description
//   coupled patch for post-processing.  Used as the base class for processor
//   and cyclic pointPatches
// SourceFiles
//   coupled_point_patch.cpp
#include "coupled_poly_patch.hpp"
namespace mousse
{
class pointBoundaryMesh;
class coupledPointPatch
{
  // Private Member Functions
    //- Disallow default construct as copy
    coupledPointPatch(const coupledPointPatch&);
    //- Disallow default assignment
    void operator=(const coupledPointPatch&);
protected:
  // Protected Member Functions
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&) = 0;
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&) = 0;
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers&, const pointField&) = 0;
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers&, const pointField&) = 0;
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&) = 0;
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&) = 0;
public:
  //- Runtime type information
  TYPE_NAME(coupledPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    coupledPointPatch(const pointBoundaryMesh& bm);
  //- Destructor
  virtual ~coupledPointPatch();
};
}  // namespace mousse
#endif

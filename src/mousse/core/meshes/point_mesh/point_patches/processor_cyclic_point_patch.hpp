#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_PROCESSOR_CYCLIC_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_PROCESSOR_CYCLIC_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicPointPatch
// Description
//   Processor patch boundary needs to be such that the ordering of
//   points in the patch is the same on both sides.
//   Looking at the creation of the faces on both sides of the processor
//   patch they need to be identical on both sides with the normals pointing
//   in opposite directions.  This is achieved by calling the reverseFace
//   function in the decomposition.  It is therefore possible to re-create
//   the ordering of patch points on the slave side by reversing all the
//   patch faces of the owner.

#include "processor_point_patch.hpp"
#include "processor_cyclic_poly_patch.hpp"


namespace mousse {

class processorCyclicPointPatch
:
  public processorPointPatch
{
  // Private data
    const processorCyclicPolyPatch& procCycPolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(processorCyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    processorCyclicPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
    //- Disallow default construct as copy
    processorCyclicPointPatch(const processorCyclicPointPatch&) = delete;
    //- Disallow default assignment
    void operator=(const processorCyclicPointPatch&) = delete;
  // Destructor
    virtual ~processorCyclicPointPatch();
  // Member functions
    //- Return message tag to use for communication
    virtual int tag() const
    {
      return procCycPolyPatch_.tag();
    }
    //- Return the underlying processorCyclicPolyPatch
    const processorCyclicPolyPatch& procCyclicPolyPatch() const
    {
      return procCycPolyPatch_;
    }
};

}  // namespace mousse

#endif


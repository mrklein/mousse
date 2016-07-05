#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_PROCESSOR_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_PROCESSOR_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorPointPatch
// Description
//   Processor patch boundary needs to be such that the ordering of
//   points in the patch is the same on both sides.
//   Looking at the creation of the faces on both sides of the processor
//   patch they need to be identical on both sides with the normals pointing
//   in opposite directions.  This is achieved by calling the reverseFace
//   function in the decomposition.  It is therefore possible to re-create
//   the ordering of patch points on the slave side by reversing all the
//   patch faces of the owner.

#include "coupled_face_point_patch.hpp"
#include "processor_poly_patch.hpp"


namespace mousse {

class processorPointPatch
:
  public coupledFacePointPatch
{
  // Private data
    const processorPolyPatch& procPolyPatch_;
    mutable labelList reverseMeshPoints_;
  // Private Member Functions
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
  TYPE_NAME(processorPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    processorPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
    //- Disallow default construct as copy
    processorPointPatch(const processorPointPatch&) = delete;
    //- Disallow default assignment
    void operator=(const processorPointPatch&) = delete;
  //- Destructor
  virtual ~processorPointPatch();
  // Member functions
    //- Return message tag to use for communication
    virtual int tag() const
    {
      return procPolyPatch_.tag();
    }
    //- Return communicator used for communication
    virtual label comm() const
    {
      return procPolyPatch_.comm();
    }
    //- Return the constraint type this pointPatch implements.
    virtual const word& constraintType() const
    {
      return type();
    }
    //- Return processor number
    int myProcNo() const
    {
      return procPolyPatch_.myProcNo();
    }
    //- Return neigbour processor number
    int neighbProcNo() const
    {
      return procPolyPatch_.neighbProcNo();
    }
    //- Is this a master patch
    bool isMaster() const
    {
      return myProcNo() < neighbProcNo();
    }
    //- Is this a slave patch
    bool isSlave() const
    {
      return !isMaster();
    }
    //- Return the underlying processorPolyPatch
    const processorPolyPatch& procPolyPatch() const
    {
      return procPolyPatch_;
    }
    //- Return mesh points in the correct order for the receiving side
    const labelList& reverseMeshPoints() const;
};

}  // namespace mousse

#endif


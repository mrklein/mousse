// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::facePointPatch
// Description
//   A pointPatch based on a polyPatch
// SourceFiles
//   face_point_patch.cpp
//   new_point_patch.cpp

#ifndef face_point_patch_hpp_
#define face_point_patch_hpp_

#include "point_patch.hpp"
#include "poly_patch.hpp"
#include "auto_ptr.hpp"

namespace mousse
{

class processorPointPatch;
class cyclicPointPatch;

class facePointPatch
:
  public pointPatch
{
protected:

  // Protected data

    //- Reference to the underlying polyPatch
    const polyPatch& polyPatch_;

  // Protected Member Functions

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

  // Declare friendship with the coupledPointPatches to allow them to extend
  // the set of points with those not associated with faces
  friend class processorPointPatch;
  friend class cyclicPointPatch;

  //- Runtime type information
  TYPE_NAME(polyPatch::typeName_());

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      facePointPatch,
      polyPatch,
      (const polyPatch& patch, const pointBoundaryMesh& bm),
      (patch, bm)
    );

  // Constructors

    //- Construct from polyPatch
    facePointPatch
    (
      const polyPatch&,
      const pointBoundaryMesh&
    );

    //- Disallow default bitwise copy construct
    facePointPatch(const facePointPatch&) = delete;

    //- Disallow default bitwise assignment
    facePointPatch& operator=(const facePointPatch&) = delete;

  // Selectors

    //- Return a pointer to a new patch created on freestore from polyPatch
    static autoPtr<facePointPatch> New
    (
      const polyPatch&,
      const pointBoundaryMesh&
    );

  //- Destructor
  virtual ~facePointPatch()
  {}

  // Member Functions

    //- Return the polyPatch
    const polyPatch& patch() const
    {
      return polyPatch_;
    }

    //- Return name
    virtual const word& name() const
    {
      return polyPatch_.name();
    }

    //- Return size
    virtual label size() const
    {
      return meshPoints().size();
    }

    //- Return the index of this patch in the pointBoundaryMesh
    virtual label index() const
    {
      return polyPatch_.index();
    }

    //- Return true if this patch field is coupled
    virtual bool coupled() const
    {
      return polyPatch_.coupled();
    }

    //- Return mesh points
    virtual const labelList& meshPoints() const
    {
      return polyPatch_.meshPoints();
    }

    //- Return pointField of points in patch
    virtual const pointField& localPoints() const
    {
      return polyPatch_.localPoints();
    }

    //- Return point unit normals
    virtual const vectorField& pointNormals() const
    {
      return polyPatch_.pointNormals();
    }

};

}  // namespace mousse
#endif

#ifndef CORE_MESHES_POINT_MESH_POINT_PATCHES_POINT_PATCH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_PATCHES_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatch
// Description
//   Basic pointPatch represents a set of points from the mesh.
// SourceFiles
//   point_patch.cpp


#include "label_list.hpp"
#include "vector_field.hpp"
#include "point_field.hpp"
#include "type_info.hpp"

namespace mousse
{

// Forward declaration of classes
class pointBoundaryMesh;
class pointConstraint;
class PstreamBuffers;

class pointPatch
{
  // Private data

    //- Reference to boundary mesh
    const pointBoundaryMesh& boundaryMesh_;

protected:

  // Protected Member Functions

    // The pointPatch geometry initialisation is called by pointBoundaryMesh
    friend class pointBoundaryMesh;

    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&)
    {}

    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&)
    {}

    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers&, const pointField&)
    {}

    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers&, const pointField&)
    {}

    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&)
    {}

    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&)
    {}

public:

  //- Runtime type information
  TYPE_NAME("basePatch");

  // Constructor

    pointPatch
    (
      const pointBoundaryMesh& bm
    )
    :
      boundaryMesh_{bm}
    {}

    //- Disallow default bitwise copy construct
    pointPatch(const pointPatch&) = delete;

    //- Disallow default bitwise assignment
    pointPatch& operator=(const pointPatch&) = delete;

  //- Destructor
  virtual ~pointPatch()
  {}

  // Member Functions

    //- Return name
    virtual const word& name() const = 0;

    //- Return size
    virtual label size() const = 0;

    //- Return the index of this patch in the pointBoundaryMesh
    virtual label index() const = 0;

    //- Return boundaryMesh reference
    const pointBoundaryMesh& boundaryMesh() const
    {
      return boundaryMesh_;
    }

    //- Return true if this patch field is coupled
    virtual bool coupled() const
    {
      return false;
    }

    //- Return mesh points
    virtual const labelList& meshPoints() const = 0;

    //- Return mesh points
    virtual const vectorField& localPoints() const = 0;

    //- Return  point normals
    virtual const vectorField& pointNormals() const = 0;

    //- Return the constraint type this pointPatch implements.
    virtual const word& constraintType() const
    {
      return word::null;
    }

    //- Accumulate the effect of constraint direction of this patch
    virtual void applyConstraint
    (
      const label /*pointi*/,
      pointConstraint&
    ) const
    {}

};

}  // namespace mousse

#endif

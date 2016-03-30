#ifndef CORE_MESHES_POINT_MESH_POINT_BOUNDARY_MESH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_BOUNDARY_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointBoundaryMesh
// Description
//   mousse::pointBoundaryMesh

#include "point_patch_list.hpp"


namespace mousse {

// Forward declaration of classes
class pointMesh;
class polyBoundaryMesh;


class pointBoundaryMesh
:
  public pointPatchList
{
  // Private data

    //- Reference to mesh
    const pointMesh& mesh_;

  // Private Member Functions

    //- Calculate the geometry for the patches (transformation tensors etc.)
    void calcGeometry();

public:

  //- Declare friendship with pointMesh
  friend class pointMesh;

  // Constructors

    //- Construct from polyBoundaryMesh
    pointBoundaryMesh
    (
      const pointMesh&,
      const polyBoundaryMesh&
    );

    //- Disallow default bitwise copy construct
    pointBoundaryMesh(const pointBoundaryMesh&) = delete;

    //- Disallow default bitwise assignment
    pointBoundaryMesh& operator=(const pointBoundaryMesh&) = delete;

  // Member functions

    //- Return the mesh reference
    const pointMesh& mesh() const
    {
      return mesh_;
    }

    //- Find patch index given a name
    label findPatchID(const word& patchName) const;

    //- Find patch indices given a name
    labelList findIndices(const keyType&, const bool useGroups) const;

    //- Correct polyBoundaryMesh after moving points
    void movePoints(const pointField&);

    //- Correct polyBoundaryMesh after topology update
    void updateMesh();

};

}  // namespace mousse

#endif

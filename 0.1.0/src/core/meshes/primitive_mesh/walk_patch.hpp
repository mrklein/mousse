#ifndef CORE_MESHES_PRIMITIVE_MESH_WALK_PATCH_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_WALK_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::walkPatch
// Description
//   Collection of static functions to do various simple patch related things.
// SourceFiles
//   walk_patch.cpp


#include "label_list.hpp"
#include "primitive_patch.hpp"

namespace mousse
{
class walkPatch
{
  // Private data
    //- Reference to patch to walk on
    const primitivePatch& pp_;
    //- Reference to zones
    const labelList& faceZone_;
    //- How to walk through faces
    const bool reverse_;
    //- Reference to list to mark off visited faces
    boolList& visited_;
    // Faces visited
    DynamicList<label> visitOrder_;
    // Index in face of vertex it was visited through
    DynamicList<label> indexInFace_;
  // Private Member Functions
    //- Get other face using v0, v1. Returns -1 if none.
    label getNeighbour
    (
      const label faceI,
      const label fp,
      const label v0,
      const label v1
    ) const;
    //- Gets labels of changed faces and enterVertices on faces.
    // Returns labels of faces changed and enterVertices on them.
    void faceToFace
    (
      const labelList& changedFaces,
      const labelList& enterVerts,
      labelList& nbrFaces,
      labelList& nbrEnterVerts
    );
public:
  CLASS_NAME("walkPatch");
  // Constructors
    //- Construct from components
    walkPatch
    (
      const primitivePatch& pp,
      const labelList& faceZone,  // Per face which zone it belongs to
      const bool reverse,         // Reverse walk
      const label faceI,          // Current face
      const label enterVertI,     // Vertex across which this face
                    // is visited.
      boolList& visited
    );
    //- Disallow default bitwise copy construct
    walkPatch(const walkPatch&) = delete;
    //- Disallow default bitwise assignment
    walkPatch& operator=(const walkPatch&) = delete;
  // Member Functions
    const DynamicList<label>& visitOrder() const
    {
      return visitOrder_;
    }
    const DynamicList<label>& indexInFace() const
    {
      return indexInFace_;
    }
};
}  // namespace mousse
#endif

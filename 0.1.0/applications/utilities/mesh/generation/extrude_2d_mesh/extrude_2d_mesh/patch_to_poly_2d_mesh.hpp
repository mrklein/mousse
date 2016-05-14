#ifndef UTILITIES_MESH_GENERATION_EXTRUDE_2D_MESH_EXTRUDE_2D_MESH_PATCH_TO_POLY_2D_MESH_PATCH_TO_POLY_2D_MESH_HPP_
#define UTILITIES_MESH_GENERATION_EXTRUDE_2D_MESH_EXTRUDE_2D_MESH_PATCH_TO_POLY_2D_MESH_PATCH_TO_POLY_2D_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchToPoly2DMesh
// Description
//   Convert a primitivePatch into a 2D polyMesh.

#include "edge_map.hpp"
#include "meshed_surface.hpp"


namespace mousse {

class patchToPoly2DMesh
{
  // Private data
    // Reference to the meshed surface
    const MeshedSurface<face>& patch_;
    const wordList& patchNames_;
    const labelList& patchSizes_;
    labelList patchStarts_;
    const EdgeMap<label>& mapEdgesRegion_;
    pointField points_;
    faceList faces_;
    labelList owner_;
    labelList neighbour_;
  // Private Member Functions
    void flipFaceOrder();
    void createNeighbours();
    labelList internalFaceOrder();
    void addPatchFacesToFaces();
    void addPatchFacesToOwner();
    void createPolyMeshComponents();
    //- Disallow default bitwise copy construct
    patchToPoly2DMesh(const patchToPoly2DMesh&);
    //- Disallow default bitwise assignment
    void operator=(const patchToPoly2DMesh&);
public:
  // Constructors
    //- Construct from a primitivePatch
    patchToPoly2DMesh
    (
      const MeshedSurface<face>& patch,
      const wordList& patchNames,
      const labelList& patchSizes,
      const EdgeMap<label>& mapEdgesRegion
    );
  //- Destructor
  ~patchToPoly2DMesh();
  // Member Functions
    // Access
      pointField& points() { return points_; }
      faceList& faces() { return faces_; }
      labelList& owner() { return owner_; }
      labelList& neighbour() { return neighbour_; }
      const wordList& patchNames() const { return patchNames_; }
      const labelList& patchSizes() const { return patchSizes_; }
      const labelList& patchStarts() const { return patchStarts_; }
    // Edit
      //- Create the mesh
      void createMesh();
};

}  // namespace mousse

#endif


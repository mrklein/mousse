#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_REPATCH_POLY_TOPO_CHANGER_REPATCH_POLY_TOPO_CHANGER_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_REPATCH_POLY_TOPO_CHANGER_REPATCH_POLY_TOPO_CHANGER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::repatchPolyTopoChanger
// Description
//   A mesh which allows changes in the patch distribution of the
//   boundary faces.  The change in patching is set using changePatchID. For a
//   boundary face, a new patch ID is given.

#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"


namespace mousse {

class repatchPolyTopoChanger
{
  // Private data
    //- The polyMesh to be repatched
    polyMesh& mesh_;
    //- Topological change to accumulated all mesh changes
    autoPtr<polyTopoChange> meshModPtr_;
  // Private Member Functions
    //- Demand driven access to polyTopoChange
    polyTopoChange& meshMod();
public:
  // Constructors
    //- Construct for given mesh
    explicit repatchPolyTopoChanger(polyMesh& mesh);
    //- Disallow default bitwise copy construct
    repatchPolyTopoChanger(const repatchPolyTopoChanger&) = delete;
    //- Disallow default bitwise assignment
    repatchPolyTopoChanger& operator=(const repatchPolyTopoChanger&) = delete;
  //- Destructor
  virtual ~repatchPolyTopoChanger()
  {}
  // Member Functions
    //- Change patches.
    void changePatches(const List<polyPatch*>& patches);
    //- Change patch ID for a boundary face. Note: patchID should be in new
    //  numbering.
    void changePatchID
    (
      const label faceID,
      const label patchID
    );
    //- Set zone ID for a face
    void setFaceZone
    (
      const label faceID,
      const label zoneID,
      const bool zoneFlip
    );
    //- Change anchor point (zero'th point of face) for a boundary face.
    void changeAnchorPoint
    (
      const label faceID,
      const label fp
    );
    //- Re-patch the mesh
    void repatch();
};

}  // namespace mousse

#endif


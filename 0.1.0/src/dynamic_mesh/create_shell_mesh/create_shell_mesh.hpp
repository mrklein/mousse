// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::createShellMesh
// Description
//   Creates mesh by extruding a patch.
// SourceFiles
//   create_shell_mesh.cpp
//   extrudes into thickness direction.
//   - bottom faces originate from reversed original faces (have turning index)
//   - top faces originate from original faces (no turning index)
#ifndef create_shell_mesh_hpp_
#define create_shell_mesh_hpp_
#include "primitive_face_patch.hpp"
#include "packed_bool_list.hpp"
#include "label_pair.hpp"
namespace mousse
{
// Forward declaration of classes
class mapPolyMesh;
class polyTopoChange;
class globalMeshData;
class createShellMesh
{
  // Private data
    //- Reference to patch to extrude
    const primitiveFacePatch& patch_;
    //- Per point on face the region
    const faceList& pointRegions_;
    //- From region to original patch point
    const labelList& regionPoints_;
    labelList cellToFaceMap_;
    labelList faceToFaceMap_;
    labelList faceToEdgeMap_;
    labelList pointToPointMap_;
  // Private Member Functions
    static void syncEdges
    (
      const globalMeshData&,
      const labelList&,
      const labelList&,
      const PackedBoolList& sameEdgeOrientation,
      const bool syncNonCollocated,
      PackedBoolList& isChangedEdge,
      DynamicList<label>& changedEdges,
      labelPairList& allEdgeData
    );
public:
  //- Runtime type information
  CLASS_NAME("createShellMesh");
  // Constructors
    //- Construct from mesh.
    createShellMesh
    (
      const primitiveFacePatch& patch,
      const faceList& pointRegions,
      const labelList& regionPoints
    );
    //- Disallow default bitwise copy construct
    createShellMesh(const createShellMesh&) = delete;
    //- Disallow default bitwise assignment
    createShellMesh& operator=(const createShellMesh&) = delete;
  // Member Functions
    // Access
      //- From region cell to patch face. Consecutively added so
      //  cell at layerI is at patchFaceI*nLayers+layerI
      const labelList& cellToFaceMap() const
      {
        return cellToFaceMap_;
      }
      //- From region face to patch face. Contains turning index:
      //      > 0 : face in same orientation as patch face. face will
      //            be in top patch
      //      < 0 : face in opposite orientation as patch face. face will
      //            be in bottom patch
      //      = 0 : for all side faces
      const labelList& faceToFaceMap() const
      {
        return faceToFaceMap_;
      }
      //- From region side-face to patch edge. -1 for non-edge faces.
      const labelList& faceToEdgeMap() const
      {
        return faceToEdgeMap_;
      }
      //- From region point to patch point.
      const labelList& pointToPointMap() const
      {
        return pointToPointMap_;
      }
    // Other
      //- Helper: calculate point regions. The point region is the
      //  same on all faces connected to a point if they can be
      //  reached through a face-edge-face walk without crossing
      //  the nonManifoldEdge.
      //  syncNonCollocated = true: edges connected through cyclic become
      //  single region. false: kept separate.
      //  pointGlobalRegions : non-compact. Guaranteed to be the same
      //  across processors.
      //  pointLocalRegions : compact.
      //  localToGlobalRegion : local to global region.
      static void calcPointRegions
      (
        const globalMeshData& globalData,
        const primitiveFacePatch& patch,
        const PackedBoolList& nonManifoldEdge,
        const bool syncNonCollocated,
        faceList& pointGlobalRegions,
        faceList& pointLocalRegions,
        labelList& localToGlobalRegion
      );
      //- Play commands into polyTopoChange to create layer mesh.
      void setRefinement
      (
        const pointField& firstLayerThickness,
        const scalar expansionRatio,
        const label nLayers,
        const labelList& topPatchID,
        const labelList& bottomPatchID,
        const labelListList& extrudeEdgePatches,
        polyTopoChange& meshMod
      );
      //- Update any locally stored mesh information.
      void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif

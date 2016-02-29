#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_FACES_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_FACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::removeFaces
// Description
//   Given list of faces to remove insert all the topology changes. Contains
//   helper function to get consistent set of faces to remove.
//   Not very well tested in parallel.
// SourceFiles
//   remove_faces.cpp
#include "pstream.hpp"
#include "hash_set.hpp"
#include "map.hpp"
#include "bool_list.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class face;
class mapPolyMesh;
class mapDistributePolyMesh;
class removeFaces
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Cosine of angles between boundary faces. Boundary faces can be
    //  merged only if angle between faces > minCos.
    const scalar minCos_;
  // Private Member Functions
    //- Change elements in cellRegion that are oldRegion to newRegion.
    //  Recurses to cell neighbours.
    void changeCellRegion
    (
      const label cellI,
      const label oldRegion,
      const label newRegion,
      labelList& cellRegion
    ) const;
    //- Changes region of connected set of faces
    label changeFaceRegion
    (
      const labelList& cellRegion,
      const boolList& removedFace,
      const labelList& nFacesPerEdge,
      const label faceI,
      const label newRegion,
      const labelList& fEdges,
      labelList& faceRegion
    ) const;
    //- Get all affected faces (including faces marked for removal)
    boolList getFacesAffected
    (
      const labelList& cellRegion,
      const labelList& cellRegionMaster,
      const labelList& facesToRemove,
      const labelHashSet& edgesToRemove,
      const labelHashSet& pointsToRemove
    ) const;
    // Topological changes
      //- Debug: write set of faces to file in obj format.
      static void writeOBJ
      (
        const indirectPrimitivePatch&,
        const fileName&
      );
      //- Merge faceLabels into single face.
      void mergeFaces
      (
        const labelList& cellRegion,
        const labelList& cellRegionMaster,
        const labelHashSet& pointsToRemove,
        const labelList& faceLabels,
        polyTopoChange& meshMod
      ) const;
      //- Get patch, zone info for faceI
      void getFaceInfo
      (
        const label faceI,
        label& patchID,
        label& zoneID,
        label& zoneFlip
      ) const;
      //- Return face with all pointsToRemove removed.
      face filterFace(const labelHashSet&, const label) const;
      //- Wrapper for meshMod.modifyFace. Reverses face if own>nei.
      void modFace
      (
        const face& f,
        const label masterFaceID,
        const label own,
        const label nei,
        const bool flipFaceFlux,
        const label newPatchID,
        const bool removeFromZone,
        const label zoneID,
        const bool zoneFlip,
        polyTopoChange& meshMod
      ) const;
public:
  //- Runtime type information
  CLASS_NAME("removeFaces");
  // Constructors
    //- Construct from mesh and min cos of angle for boundary faces
    //  to be considered aligned. Set to >= 1 to disable checking
    //  and always merge (if on same patch)
    removeFaces(const polyMesh&, const scalar minCos);
    //- Disallow default bitwise copy construct
    removeFaces(const removeFaces&) = delete;
    //- Disallow default bitwise assignment
    removeFaces& operator=(const removeFaces&) = delete;
  // Member Functions
    //- Find faces including those with cells which have the same mastercell
    //  Given set of faces to pierce calculates:
    //  - region for connected cells
    //  - mastercell for each region. This is the lowest numbered cell
    //    of all cells that get merged.
    //  - new set of faces which contains input set + additional ones
    //    where cells on both sides would have same mastercell.
    //  Returns number of regions.
    label compatibleRemoves
    (
      const labelList& inPiercedFaces,
      labelList& cellRegion,
      labelList& cellRegionMaster,
      labelList& outPiercedFaces
    ) const;
    //- Play commands into polyTopoChange to remove faces.
    void setRefinement
    (
      const labelList& piercedFaces,
      const labelList& cellRegion,
      const labelList& cellRegionMaster,
      polyTopoChange&
    ) const;
    //- Force recalculation of locally stored data on topological change
    void updateMesh(const mapPolyMesh&)
    {}
    //- Force recalculation of locally stored data for mesh distribution
    void distribute(const mapDistributePolyMesh&)
    {}
};
}  // namespace mousse
#endif

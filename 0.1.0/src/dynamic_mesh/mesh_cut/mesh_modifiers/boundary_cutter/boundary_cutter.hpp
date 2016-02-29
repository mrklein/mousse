#ifndef DYNAMIC_MESH_MESH_CUT_MESH_MODIFIERS_BOUNDARY_CUTTER_BOUNDARY_CUTTER_HPP_
#define DYNAMIC_MESH_MESH_CUT_MESH_MODIFIERS_BOUNDARY_CUTTER_BOUNDARY_CUTTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boundaryCutter
// Description
//   Does modifications to boundary faces.
//   Does
//   - move boundary points
//   - split boundary edges (multiple per edge if necessary)
//   - face-centre decomposes boundary faces
//   - diagonal split of boundary faces
// SourceFiles
//   boundary_cutter.cpp
#include "map.hpp"
#include "label_list.hpp"
#include "edge.hpp"
#include "type_info.hpp"
#include "label_pair.hpp"
namespace mousse
{
// Forward declaration of classes
class polyTopoChange;
class mapPolyMesh;
class polyMesh;
class face;
class boundaryCutter
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Per edge sorted (start to end) list of points added.
    HashTable<labelList, edge, Hash<edge> > edgeAddedPoints_;
    //- Per face the mid point added.
    Map<label> faceAddedPoint_;
  // Private Member Functions
    //- Get patch and zone info for face
    void getFaceInfo
    (
      const label faceI,
      label& patchID,
      label& zoneID,
      label& zoneFlip
    ) const;
    //- Add cuts of edges to face
    face addEdgeCutsToFace(const label faceI, const Map<labelList>&) const;
    //- Splits faces with multiple cut edges. Return true if anything split.
    bool splitFace
    (
      const label faceI,
      const Map<point>& pointToPos,
      const Map<labelList>& edgeToAddedPoints,
      polyTopoChange& meshMod
    ) const;
    //- Add/modify faceI for new vertices.
    void addFace
    (
      const label faceI,
      const face& newFace,
      bool& modifiedFace,     // have we already 'used' faceI?
      polyTopoChange& meshMod
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("boundaryCutter");
  // Constructors
    //- Construct from mesh
    boundaryCutter(const polyMesh& mesh);
    //- Disallow default bitwise copy construct
    boundaryCutter(const boundaryCutter&) = delete;
    //- Disallow default bitwise assignment
    boundaryCutter& operator=(const boundaryCutter&) = delete;
  //- Destructor
  ~boundaryCutter();
  // Member Functions
    // Edit
      //- Do actual cutting with cut description. Inserts mesh changes
      //  into meshMod.
      //  pointToPos : new position for selected points
      //  edgeToCuts : per edge set of points that need to be introduced
      //  faceToSplit : per face the diagonal split
      //  faceToFeaturePoint : per face the feature point. Triangulation
      //                       around this feature point.
      void setRefinement
      (
        const Map<point>& pointToPos,
        const Map<List<point> >& edgeToCuts,
        const Map<labelPair>& faceToSplit,
        const Map<point>& faceToFeaturePoint,
        polyTopoChange& meshMod
      );
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
    // Access
      //- Per edge a sorted list (start to end) of added points.
      const HashTable<labelList, edge, Hash<edge> >& edgeAddedPoints()
      const
      {
        return edgeAddedPoints_;
      }
      //- Per face the mid point added.
      const Map<label>& faceAddedPoint() const
      {
        return faceAddedPoint_;
      }
};
}  // namespace mousse
#endif

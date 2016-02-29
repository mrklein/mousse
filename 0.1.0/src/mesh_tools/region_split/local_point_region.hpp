#ifndef MESH_TOOLS_REGION_SPLIT_LOCAL_POINT_REGION_HPP_
#define MESH_TOOLS_REGION_SPLIT_LOCAL_POINT_REGION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::localPointRegion
// Description
//   Takes mesh with 'baffles' (= boundary faces sharing points).
//   Determines for selected points on boundary faces the 'point region' it is
//   connected to. Each region can be visited by a cell-face-cell walk.
//   Used in duplicating points after splitting baffles.
//   Regions are not consecutive per processor. They will be -1..nRegions_.
//   Note: coupled boundaries (cyclics, parallel) not fully tested.
// SourceFiles
//   local_point_region.cpp
#include "type_info.hpp"
#include "map.hpp"
#include "label_list.hpp"
#include "hash_set.hpp"
#include "face_list.hpp"
#include "bool_list.hpp"
#include "label_pair.hpp"
namespace mousse
{
// Forward declaration of classes
class primitiveMesh;
class polyMesh;
class face;
class mapPolyMesh;
class localPointRegion
{
  // Private data
    ////- (global) number of regions
    //label nRegions_;
    //- Per point that is to duplicated to the local index
    Map<label> meshPointMap_;
    //- Per local point the regions it is in
    labelListList pointRegions_;
    //- Per face that uses a duplicated point the local index
    Map<label> meshFaceMap_;
    //- Per face the region of its points
    faceList faceRegions_;
  // Private Member Functions
    //- Given minimum cell the points on a face are connected to
    //  determine the points to be duplicated.
    void countPointRegions
    (
      const polyMesh& mesh,
      const boolList& candidatePoint,
      const Map<label>& candidateFace,
      faceList& minRegion
    );
    //- Do all: calculate points that need to be duplicated.
    void calcPointRegions
    (
      const polyMesh& mesh,
      boolList& candidatePoint
    );
    //- Check if two faces are equal. If forward = false checks f1 in
    //  reverse order.
    static bool isDuplicate
    (
      const face& f0,
      const face& f1,
      const bool forward
    );
public:
  //- Runtime type information
  CLASS_NAME("localPointRegion");
  // Constructors
    //- Construct from mesh. Assumes all non-coupled boundary points
    //  are candidates for duplication
    localPointRegion(const polyMesh& mesh);
    //- Construct from mesh and candidate points for duplication
    localPointRegion
    (
      const polyMesh& mesh,
      const labelList& candidatePoints
    );
  // Member Functions
    // Static Member Functions
      //- Helper routine to find baffles (two boundary faces using the
      //  same points but in reverse order)
      //  Gets list of (boundary!) faces to check. Returns labelList
      //  of same size as the input list
      //  with -1 or index of other face in the input list.
      //  Does not handle duplicate faces on both sides of processor patch
      static labelList findDuplicateFaces
      (
        const primitiveMesh&,
        const labelList&
      );
      //- Helper routine to find all baffles (two boundary faces
      //  using the same points but in reverse order)
      static labelPairList findDuplicateFacePairs(const polyMesh&);
    // Access
      ////- Global number of regions. TBD. regions not compacted yet.
      //label nRegions() const
      //{
      //    return nRegions_;
      //}
      //- Per point that is to be duplicated the local index
      const Map<label>& meshPointMap() const
      {
        return meshPointMap_;
      }
      //- Per local point the regions it is in
      const labelListList& pointRegions() const
      {
        return pointRegions_;
      }
      //- Per face that uses a duplicated point the local index
      const Map<label>& meshFaceMap() const
      {
        return meshFaceMap_;
      }
      //- Per face the region of its points
      const faceList& faceRegions() const
      {
        return faceRegions_;
      }
    // Edit
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif

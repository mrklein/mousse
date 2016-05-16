#ifndef DYNAMIC_MESH_SLIDING_INTERFACE_ENRICHED_PATCH_ENRICHED_PATCH_HPP_
#define DYNAMIC_MESH_SLIDING_INTERFACE_ENRICHED_PATCH_ENRICHED_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::enrichedPatch
// Description
//   The enriched patch contains a double set of faces from the two
//   sides of the sliding interface before the cutting.
//   The patch basically consists of two over-lapping sets of faces sitting
//   on a common point support, where every edge may be shared by more than
//   2 faces.  The patch points are collected in a map.  Additional
//   information needed for cutting is the point insertion into every edge
//   of master and slave.
//   Note:
//   If new points are created during master-slave edge cutting, they
//   should be registred with the pointMap.

#include "primitive_face_patch.hpp"
#include "map.hpp"
#include "point.hpp"


namespace mousse {

class enrichedPatch
{
  // Private data
    //- Reference to master patch
    const primitiveFacePatch& masterPatch_;
    //- Reference to slave patch
    const primitiveFacePatch& slavePatch_;
    //- Map of points supporting patch faces
    mutable Map<point> pointMap_;
    //- Has the point map been completed?
    mutable bool pointMapComplete_;
    //- Map of point merges
    mutable Map<label> pointMergeMap_;
    //- Slave point point hits
    const labelList& slavePointPointHits_;
    //- Slave point edge hits
    const labelList& slavePointEdgeHits_;
    //- Slave point face hits
    const List<objectHit>& slavePointFaceHits_;
  // Demand-driven private data
    //- Enriched patch
    mutable faceList* enrichedFacesPtr_;
    //- Mesh points
    mutable labelList* meshPointsPtr_;
    //- Local faces
    mutable faceList* localFacesPtr_;
    //- Local points
    mutable pointField* localPointsPtr_;
    //- Point-point addressing
    mutable labelListList* pointPointsPtr_;
    // Master point addressing
    mutable Map<labelList>* masterPointFacesPtr_;
    // Cut faces and addressing
      //- Cut faces
      mutable faceList* cutFacesPtr_;
      //- Cut face master
      //  - the face on the master patch for internal faces
      //  - the creator face for boundary face
      mutable labelList* cutFaceMasterPtr_;
      //- Cut face slave
      //  - the face on the slave patch for internal faces
      //  - -1 for boundary face
      mutable labelList* cutFaceSlavePtr_;
  // Private Member Functions
    // Creation of demand-driven private data
      //- Calculate point merge map
      void calcPointMergeMap() const;
      //- Complete point map
      void completePointMap() const;
      //- Calculate mesh points
      void calcMeshPoints() const;
      //- Calculate local points
      void calcLocalPoints() const;
      //- Calculate local faces
      void calcLocalFaces() const;
      //- Calculate point-point addressing
      void calcPointPoints() const;
      //- Calculate master point addressing
      void calcMasterPointFaces() const;
      //- Calculate cut faces
      void calcCutFaces() const;
      //- Clear cut faces
      void clearCutFaces();
      //- Clear out demand-driven data
      void clearOut();
  // Static data members
    //- Estimated ratio of original-to-enriched face size
    static const label enrichedFaceRatio_;
    //- Estimated number of master face hits by slave points
    static const label nFaceHits_;
    //- Size of face on which the check is forced
    static const label maxFaceSizeDebug_;
public:
  // Static data members
  CLASS_NAME("enrichedPatch");
  // Constructors
    //- Construct from components
    enrichedPatch
    (
      const primitiveFacePatch& masterPatch,
      const primitiveFacePatch& slavePatch,
      const labelList& slavePointPointHits,
                      // -1 or common point snapped to
      const labelList& slavePointEdgeHits,
                      // -1 or common edge snapped to
      const List<objectHit>& slavePointFaceHits
                      // master face snapped to
    );
    //- Disallow default bitwise copy construct
    enrichedPatch(const enrichedPatch&) = delete;
    //- Disallow default bitwise assignment
    enrichedPatch& operator=(const enrichedPatch&) = delete;
  //- Destructor
  ~enrichedPatch();
  // Member Functions
    // Access
      //- Return non-const access to point map to add points
      Map<point>& pointMap();
      //- Return map of points
      const Map<point>& pointMap() const;
      //- Return map of point merges
      Map<label>& pointMergeMap()
      {
        return pointMergeMap_;
      }
      //- Return map of point merges
      const Map<label>& pointMergeMap() const
      {
        return pointMergeMap_;
      }
    // Topological data
      //- Calculate enriched faces
      void calcEnrichedFaces
      (
        const labelListList& pointsIntoMasterEdges,
        const labelListList& pointsIntoSlaveEdges,
        const pointField& projectedSlavePoints
      );
      //- Return enriched faces
      const faceList& enrichedFaces() const;
      //- Return mesh points
      const labelList& meshPoints() const;
      //- Return local faces
      const faceList& localFaces() const;
      //- Return local points
      const pointField& localPoints() const;
      //- Return point-point addressing
      const labelListList& pointPoints() const;
      //- Master point face addressing
      const Map<labelList>& masterPointFaces() const;
    // Cut faces
      //- Return list of cut faces
      const faceList& cutFaces() const;
      //- Return cut face master list
      const labelList& cutFaceMaster() const;
      //- Return cut face slave list
      const labelList& cutFaceSlave() const;
    //- Check if the patch is fully supported
    bool checkSupport() const;
    //- Debugging: dump graphical representation to obj format file
    void writeOBJ(const fileName&) const;
};

}  // namespace mousse

#endif


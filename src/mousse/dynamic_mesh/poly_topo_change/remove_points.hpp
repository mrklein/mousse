#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_POINTS_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_POINTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::removePoints
// Description
//   Removes selected points from mesh and updates faces using these
//   points.

#include "type_info.hpp"
#include "bool_list.hpp"
#include "point_field.hpp"
#include "face_list.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class mapPolyMesh;
class face;


class removePoints
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Whether undoable
    const bool undoable_;
    //- If undoable: deleted points
    pointField savedPoints_;
    //- If undoable: per stored face the original mesh face label
    labelList savedFaceLabels_;
    //- If undoable: per stored face the vertices. Negative indices
    //  refer to deletedPoints_
    faceList savedFaces_;
  // Private Member Functions
    //- Change the vertices of the face whilst keeping everything else
    //  (patch, zone) the same.
    void modifyFace
    (
      const label faceI,
      const face&,
      polyTopoChange&
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("removePoints");
  // Constructors
    //- Construct from mesh
    removePoints(const polyMesh& mesh, const bool undoable = false);
    //- Disallow default bitwise copy construct
    removePoints(const removePoints&) = delete;
    //- Disallow default bitwise assignment
    removePoints& operator=(const removePoints&) = delete;
  // Member Functions
      //- If undoable: affected face labels. Already restored faces
      //  will be -1.
      const labelList& savedFaceLabels() const
      {
        return savedFaceLabels_;
      }
    // Helper functions
      //- Mark in pointCanBeDeleted the points that can be deleted
      //  (parallel synchronised) and returns the global number of these
      //  points. (this number is the global number before synchronisation
      //  so might be off!)
      //  A point can be deleted if
      //  - it is not used by any edge.
      //  or
      //  - is used by only two edges
      //  - these two edges are sufficiently in line (cos > minCos)
      //  - all processors agree that point can be deleted.
      label countPointUsage
      (
        const scalar minCos,
        boolList& pointCanBeDeleted
      ) const;
    // Topology changes
      //- Play commands into polyTopoChange to remove points. Gets
      //  boolList (output of countPointUsage) as input.
      //  Does no check for whether resulting face is legal.
      //  Since pointCanBeDeleted is synced all coupled faces should
      //  decide the same.
      void setRefinement(const boolList&, polyTopoChange&);
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
      //- Given set of faces to restore calculates a consistent set of
      //  saved faces (indices into savedFaces_) and saved vertices
      //  (indices into savedPoints_) to restore. The undoFaces have to
      //  be synced.
      void getUnrefimentSet
      (
        const labelList& undoFaces,
        labelList& localFaces,
        labelList& localPoints
      ) const;
      //- Restore selected faces and vertices.
      void setUnrefinement
      (
        const labelList& localFaces,
        const labelList& localPoints,
        polyTopoChange&
      );
};

}  // namespace mousse

#endif


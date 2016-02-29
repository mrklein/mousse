#ifndef UTILITIES_MESH_ADVANCED_MODIFY_MESH_CELL_SPLITTER_HPP_
#define UTILITIES_MESH_ADVANCED_MODIFY_MESH_CELL_SPLITTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSplitter
// Description
//   Does pyramidal decomposition of selected cells. So all faces will become
//   base of pyramid with as top a user-supplied point (usually the cell centre)
// SourceFiles
//   cell_splitter.cpp

#include "map.hpp"
#include "edge.hpp"
#include "type_info.hpp"

namespace mousse
{
// Forward declaration of classes
class polyTopoChange;
class mapPolyMesh;
class polyMesh;
class cellSplitter
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Per cell the mid point added.
    Map<label> addedPoints_;
  // Private Member Functions
    //- Get patch and zone info for face
    void getFaceInfo
    (
      const label faceI,
      label& patchID,
      label& zoneID,
      label& zoneFlip
    ) const;
    //- Find the new owner (if any) of the face.
    label newOwner
    (
      const label faceI,
      const Map<labelList>& cellToCells
    ) const;
    //- Find the new neighbour (if any) of the face.
    label newNeighbour
    (
      const label faceI,
      const Map<labelList>& cellToCells
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("cellSplitter");
  // Constructors
    //- Construct from mesh
    cellSplitter(const polyMesh& mesh);
    //- Disallow default bitwise copy construct
    cellSplitter(const cellSplitter&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellSplitter&) = delete;
  //- Destructor
  ~cellSplitter();
  // Member Functions
    // Edit
      //- Insert mesh changes into meshMod.
      //  cellToMidPoint : cell to cut and position of its new midpoint
      void setRefinement
      (
        const Map<point>& cellToMidPoint,
        polyTopoChange& meshMod
      );
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
    // Access
      //- Per cell the mid point added.
      const Map<label>& addedPoints() const
      {
        return addedPoints_;
      }
};
}  // namespace mousse
#endif

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blockMesh
// Description
//   A multi-block mesh generator
// Note
//   The vertices, cells and patches for filling the blocks are demand-driven.
// SourceFiles
//   block_mesh.cpp
//   block_mesh_check.cpp
//   block_mesh_create.cpp
//   block_mesh_merge.cpp
//   block_mesh_topology.cpp
#ifndef block_mesh_hpp_
#define block_mesh_hpp_
#include "block_list.hpp"
#include "poly_mesh.hpp"
#include "iodictionary.hpp"
#include "curved_edge_list.hpp"
namespace mousse
{
class blockMesh
:
  public blockList
{
  // Private data
    //- Switch for verbose output
    static bool verboseOutput;
    //- Point field defining the block mesh (corners)
    pointField blockPointField_;
    //- The list of curved edges
    curvedEdgeList edges_;
    //- The scaling factor to convert to metres
    scalar scaleFactor_;
    //- The blocks themselves (the topology) as a polyMesh
    polyMesh* topologyPtr_;
    label nPoints_;
    //- The sum of all cells in each block
    label nCells_;
    //- The point offset added to each block
    labelList blockOffsets_;
    //- The merge points information
    labelList mergeList_;
    mutable pointField points_;
    mutable cellShapeList cells_;
    mutable faceListList patches_;
  // Private Member Functions
    bool blockLabelsOK
    (
      const label blockLabel,
      const pointField& points,
      const cellShape& blockShape
    ) const;
    bool patchLabelsOK
    (
      const label patchLabel,
      const pointField& points,
      const faceList& patchShapes
    ) const;
    bool readPatches
    (
      const dictionary& meshDescription,
      faceListList& tmpBlocksPatches,
      wordList& patchNames,
      wordList& patchTypes,
      wordList& nbrPatchNames
    );
    bool readBoundary
    (
      const dictionary& meshDescription,
      wordList& patchNames,
      faceListList& tmpBlocksPatches,
      PtrList<dictionary>& patchDicts
    );
    void createCellShapes(cellShapeList& tmpBlockCells);
    polyMesh* createTopology(const IOdictionary&, const word& regionName);
    void checkBlockMesh(const polyMesh&) const;
    //- Determine the merge info and the final number of cells/points
    void calcMergeInfo();
    //- Determine the merge info and the final number of cells/points
    void calcMergeInfoFast();
    faceList createPatchFaces(const polyPatch& patchTopologyFaces) const;
    void createPoints() const;
    void createCells() const;
    void createPatches() const;
    //- As copy (not implemented)
    blockMesh(const blockMesh&);
public:
  // Static data members
    ClassName("blockMesh");
  // Constructors
    //- Construct from IOdictionary
    blockMesh(const IOdictionary&, const word& regionName);
  //- Destructor
  ~blockMesh();
  // Member Functions
    // Access
      //- Reference to point field defining the block mesh
      //  these points have not been scaled by scaleFactor
      const pointField& blockPointField() const;
      //- Return the blockMesh topology as a polyMesh
      const polyMesh& topology() const;
      //- Return the curved edges
      const curvedEdgeList& edges() const
      {
        return edges_;
      }
      //- The scaling factor used to convert to metres
      scalar scaleFactor() const;
      //- The points for the entire mesh
      //  these points have been scaled by scaleFactor
      const pointField& points() const;
      //- Return cell shapes list
      const cellShapeList& cells() const;
      //- Return the patch face lists
      const faceListList& patches() const;
      //- Get patch information from the topology mesh
      PtrList<dictionary> patchDicts() const;
      //- Return patch names
      wordList patchNames() const;
      //- Number of blocks with specified zones
      label numZonedBlocks() const;
    // Edit
      //- Clear geometry (internal points, cells, boundaryPatches)
      void clearGeom();
      //- Enable/disable verbose information about the progress
      static void verbose(const bool on=true);
    // Write
      //- Writes edges of blockMesh in OBJ format.
      void writeTopology(Ostream&) const;
};
}  // namespace mousse
#endif

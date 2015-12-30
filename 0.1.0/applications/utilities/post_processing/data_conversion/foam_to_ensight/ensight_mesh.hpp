// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightMesh
// Description
// SourceFiles
//   ensight_mesh.cpp
#ifndef ensight_mesh_hpp_
#define ensight_mesh_hpp_
#include "cell_sets.hpp"
#include "face_sets.hpp"
#include "hash_table.hpp"
#include "hash_set.hpp"
#include "packed_bool_list.hpp"
#include "word_re_list.hpp"
#include "scalar_field.hpp"
#include "cell_shape_list.hpp"
#include "cell_list.hpp"
#include <fstream>
namespace mousse
{
class fvMesh;
class argList;
class globalIndex;
class ensightStream;
class ensightMesh
{
public:
    class nFacePrimitives
    {
    public:
      label nTris;
      label nQuads;
      label nPolys;
      nFacePrimitives()
      :
        nTris(0),
        nQuads(0),
        nPolys(0)
      {}
    };
private:
  // Private data
    //- Reference to the OpenFOAM mesh
    const fvMesh& mesh_;
    //- Suppress patches
    const bool noPatches_;
    //- Output selected patches only
    const bool patches_;
    const wordReList patchPatterns_;
    //- Output selected faceZones
    const bool faceZones_;
    const wordReList faceZonePatterns_;
    //- Set binary file output
    const bool binary_;
    //- The ensight part id for the first patch
    label patchPartOffset_;
    cellSets meshCellSets_;
    List<faceSets> boundaryFaceSets_;
    wordList allPatchNames_;
    wordHashSet patchNames_;
    HashTable<nFacePrimitives> nPatchPrims_;
    // faceZone - related variables
    List<faceSets> faceZoneFaceSets_;
    wordHashSet faceZoneNames_;
    HashTable<nFacePrimitives> nFaceZonePrims_;
    //- Per boundary face whether to include or not
    PackedBoolList boundaryFaceToBeIncluded_;
    // Parallel merged points
      //- Global numbering for merged points
      autoPtr<globalIndex> globalPointsPtr_;
      //- From mesh point to global merged point
      labelList pointToGlobal_;
      //- Local points that are unique
      labelList uniquePointMap_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    ensightMesh(const ensightMesh&);
    //- Disallow default bitwise assignment
    void operator=(const ensightMesh&);
    void writePoints
    (
      const scalarField& pointsComponent,
      ensightStream& ensightGeometryFile
    ) const;
    cellShapeList map
    (
      const cellShapeList& cellShapes,
      const labelList& prims,
      const labelList& pointToGlobal
    ) const;
    cellShapeList map
    (
      const cellShapeList& cellShapes,
      const labelList& hexes,
      const labelList& wedges,
      const labelList& pointToGlobal
    ) const;
    void writePrims
    (
      const cellShapeList& cellShapes,
      ensightStream& ensightGeometryFile
    ) const;
    void writePolysNFaces
    (
      const labelList& polys,
      const cellList& cellFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writePolysNPointsPerFace
    (
      const labelList& polys,
      const cellList& cellFaces,
      const faceList& faces,
      ensightStream& ensightGeometryFile
    ) const;
    void writePolysPoints
    (
      const labelList& polys,
      const cellList& cellFaces,
      const faceList& faces,
      const labelList& faceOwner,
      ensightStream& ensightGeometryFile
    ) const;
    void writeAllPolys
    (
      const labelList& pointToGlobal,
      ensightStream& ensightGeometryFile
    ) const;
    void writeAllPrims
    (
      const char* key,
      const label nPrims,
      const cellShapeList& cellShapes,
      ensightStream& ensightGeometryFile
    ) const;
    void writeFacePrims
    (
      const faceList& patchFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writeAllFacePrims
    (
      const char* key,
      const labelList& prims,
      const label nPrims,
      const faceList& patchFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writeNSidedNPointsPerFace
    (
      const faceList& patchFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writeNSidedPoints
    (
      const faceList& patchFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writeAllNSided
    (
      const labelList& prims,
      const label nPrims,
      const faceList& patchFaces,
      ensightStream& ensightGeometryFile
    ) const;
    void writeAllPoints
    (
      const label ensightPartI,
      const word& ensightPartName,
      const pointField& uniquePoints,
      const label nPoints,
      ensightStream& ensightGeometryFile
    ) const;
public:
  // Constructors
    //- Construct from fvMesh
    ensightMesh
    (
      const fvMesh& mesh,
      const bool noPatches,
      const bool patches,
      const wordReList& patchPatterns,
      const bool faceZones,
      const wordReList& faceZonePatterns,
      const bool binary
    );
  //- Destructor
  ~ensightMesh();
  // Member Functions
    // Access
      const fvMesh& mesh() const
      {
        return mesh_;
      }
      const cellSets& meshCellSets() const
      {
        return meshCellSets_;
      }
      const List<faceSets>& boundaryFaceSets() const
      {
        return boundaryFaceSets_;
      }
      const wordList& allPatchNames() const
      {
        return allPatchNames_;
      }
      const wordHashSet& patchNames() const
      {
        return patchNames_;
      }
      const HashTable<nFacePrimitives>& nPatchPrims() const
      {
        return nPatchPrims_;
      }
      const List<faceSets>& faceZoneFaceSets() const
      {
        return faceZoneFaceSets_;
      }
      const wordHashSet& faceZoneNames() const
      {
        return faceZoneNames_;
      }
      const HashTable<nFacePrimitives>& nFaceZonePrims() const
      {
        return nFaceZonePrims_;
      }
      //- The ensight part id for the first patch
      label patchPartOffset() const
      {
        return patchPartOffset_;
      }
    // Parallel point merging
      //- Global numbering for merged points
      const globalIndex& globalPoints() const
      {
        return globalPointsPtr_();
      }
      //- From mesh point to global merged point
      const labelList& pointToGlobal() const
      {
        return pointToGlobal_;
      }
      //- Local points that are unique
      const labelList& uniquePointMap() const
      {
        return uniquePointMap_;
      }
  // Other
    //- Update for new mesh
    void correct();
    //- When exporting faceZones, check if a given face has to be included
    //  or not (i.e. faces on processor boundaries)
    bool faceToBeIncluded(const label faceI) const;
    //- Helper to cause barrier. Necessary on Quadrics.
    static void barrier();
  // I-O
    void write
    (
      const fileName& postProcPath,
      const word& prepend,
      const label timeIndex,
      const bool meshMoving,
      Ostream& ensightCaseFile
    ) const;
};
}  // namespace mousse
#endif

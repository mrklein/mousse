#ifndef DYNAMIC_MESH_POLY_MESH_FILTER_POLY_MESH_FILTER_HPP_
#define DYNAMIC_MESH_POLY_MESH_FILTER_POLY_MESH_FILTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyMeshFilter
// Description
//   Remove the edges and faces of a polyMesh whilst satisfying the given mesh
//   quality criteria.
//   Works on a copy of the mesh.
// SourceFiles
//   poly_mesh_filter.cpp
//   poly_mesh_filter_templates.cpp
#include "iodictionary.hpp"
#include "time.hpp"
#include "list.hpp"
#include "auto_ptr.hpp"
#include "scalar_field.hpp"
#include "poly_mesh_filter_settings.hpp"
namespace mousse
{
class polyMesh;
class fvMesh;
class PackedBoolList;
class faceSet;
class polyMeshFilter
:
  private polyMeshFilterSettings
{
  // Private data
    //- Reference to the original mesh
    const fvMesh& mesh_;
    //- Copy of the original mesh to perform the filtering on
    autoPtr<fvMesh> newMeshPtr_;
    //- Original point priorities. If a point has a higher priority than
    //  another point then the edge between them collapses towards the
    //  point with the higher priority. (e.g. 2 is a higher priority than 1)
    labelList originalPointPriority_;
    //- Point priority associated with the new mesh
    autoPtr<labelList> pointPriority_;
    //- The minimum edge length for each edge
    scalarField minEdgeLen_;
    //- The face filter factor for each face
    scalarField faceFilterFactor_;
  // Private Member Functions
    template<typename T>
    static void updateSets(const mapPolyMesh& map);
    template<typename T>
    static void copySets(const polyMesh& oldMesh, const polyMesh& newMesh);
    label filterFacesLoop(const label nOriginalBadFaces);
    label filterFaces
    (
      polyMesh& newMesh,
      scalarField& newMeshFaceFilterFactor,
      labelList& origToCurrentPointMap
    );
    label filterEdges
    (
      polyMesh& newMesh,
      scalarField& newMeshMinEdgeLen,
      labelList& origToCurrentPointMap
    );
    //- Increment pointErrorCount for points attached to a bad face
    void updatePointErrorCount
    (
      const PackedBoolList& isErrorPoint,
      const labelList& oldToNewMesh,
      labelList& pointErrorCount
    ) const;
    //- Given the new points that are part of bad faces, and a map from the
    //  old mesh points to the new mesh points, relax minEdgeLen_
    void checkMeshEdgesAndRelaxEdges
    (
      const polyMesh& newMesh,
      const labelList& oldToNewMesh,
      const PackedBoolList& isErrorPoint,
      const labelList& pointErrorCount
    );
    //- Given the new points that are part of bad faces, and a map from the
    //  old mesh points to the new mesh points, relax faceFilterFactor_
    void checkMeshFacesAndRelaxEdges
    (
      const polyMesh& newMesh,
      const labelList& oldToNewMesh,
      const PackedBoolList& isErrorPoint,
      const labelList& pointErrorCount
    );
    // Mark boundary points
    // boundaryPoint:
    // + -1 : point not on boundary
    // +  0 : point on a real boundary
    // + >0 : point on a processor patch with that ID
    // @todo Need to mark boundaryEdges as well, as an edge may have two
    //       boundary points but not itself lie on a boundary
    void updatePointPriorities
    (
      const polyMesh& newMesh,
      const labelList& pointMap
    );
    //- Print min/mean/max data for a field
    void printScalarFieldStats
    (
      const string desc,
      const scalarField& fld
    ) const;
    //- Update minEdgeLen_ for the new mesh based upon the movement of the
    //  old points to the new points
    void mapOldMeshEdgeFieldToNewMesh
    (
      const polyMesh& newMesh,
      const labelList& pointMap,
      scalarField& newMeshMinEdgeLen
    ) const;
    //- Update faceFilterFactor_ for the new mesh based upon the movement
    //  of the old faces to the new faces
    void mapOldMeshFaceFieldToNewMesh
    (
      const polyMesh& newMesh,
      const labelList& faceMap,
      scalarField& newMeshFaceFilterFactor
    ) const;
    //- Maintain a map of the original mesh points to the latest version of
    //  the filtered mesh.
    void updateOldToNewPointMap
    (
      const labelList& currToNew,
      labelList& origToCurrentPointMap
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("polyMeshFilter");
  // Constructors
    //- Construct from fvMesh
    explicit polyMeshFilter(const fvMesh& mesh);
    //- Construct from fvMesh and a label list of point priorities
    polyMeshFilter(const fvMesh& mesh, const labelList& pointPriority);
    //- Disallow default bitwise copy construct
    polyMeshFilter(const polyMeshFilter&) = delete;
    //- Disallow default bitwise assignment
    polyMeshFilter& operator=(const polyMeshFilter&) = delete;
  //- Destructor
  ~polyMeshFilter();
  // Member Functions
    // Access
      //- Return reference to the filtered mesh. Does not check if the
      //  mesh has actually been filtered.
      const autoPtr<fvMesh>& filteredMesh() const;
      //- Return the new pointPriority list.
      const autoPtr<labelList>& pointPriority() const;
    // Edit
      //- Return a copy of an fvMesh
      static autoPtr<fvMesh> copyMesh(const fvMesh& mesh);
      //- Copy loaded topoSets from the old mesh to the new mesh
      static void copySets
      (
        const polyMesh& oldMesh,
        const polyMesh& newMesh
      );
      //- Update the loaded topoSets
      static void updateSets(const mapPolyMesh& map);
      //- Filter edges and faces
      label filter(const label nOriginalBadFaces);
      //- Filter all faces that are in the face set
      label filter(const faceSet& fSet);
      //- Filter edges only.
      label filterEdges(const label nOriginalBadFaces);
};
}  // namespace mousse
#ifdef NoRepository
#   include "poly_mesh_filter_templates.cpp"
#endif
#endif

#ifndef CORE_MESHES_PRIMITIVE_MESH_TPRIMITIVE_PATCH_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_TPRIMITIVE_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PrimitivePatch
// Description
//   A list of faces which address into the list of points.
//   The class is templated on the face type (e.g. triangle, polygon etc.)
//   and on the list type of faces and points so that it can refer to
//   existing lists using UList and const pointField& or hold the storage
//   using List and pointField.
// SourceFiles
//   _primitive_patch_addressing.cpp
//   _primitive_patch_bdry_points.cpp
//   _primitive_patch.cpp
//   _primitive_patch_check.cpp
//   _primitive_patch_clear.cpp
//   _primitive_patch_edge_loops.cpp
//   _primitive_patch_local_point_order.cpp
//   _primitive_patch_mesh_data.cpp
//   _primitive_patch_mesh_edges.cpp
//   _primitive_patch_name.cpp
//   _primitive_patch_point_addressing.cpp
//   _primitive_patch_project_points.cpp


#include "bool_list.hpp"
#include "label_list.hpp"
#include "edge_list.hpp"
#include "point.hpp"
#include "intersection.hpp"
#include "hash_set.hpp"
#include "object_hit.hpp"

namespace mousse
{

class face;

template<class T> class Map;
TEMPLATE_NAME(PrimitivePatch);
template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType=point
>
class PrimitivePatch
:
  public PrimitivePatchName,
  public FaceList<Face>
{
public:

  // Public typedefs
    typedef Face FaceType;
    typedef FaceList<Face> FaceListType;
    typedef PointField PointFieldType;

  // Public data types
    //- Enumeration defining the surface type. Used in check routines.
    enum surfaceTopo
    {
      MANIFOLD,
      OPEN,
      ILLEGAL
    };

private:
  // Private data

    //- Reference to global list of points
    PointField points_;

  // Demand driven private data

    //- Edges of the patch; address into local point list;
    //  sorted with internal edges first in upper-triangular order
    //  and external edges last.
    mutable edgeList* edgesPtr_;

    //- Which part of edgesPtr_ is internal edges.
    mutable label nInternalEdges_;

    //- Boundary point labels, addressing into local point list
    mutable labelList* boundaryPointsPtr_;

    //- Face-face addressing
    mutable labelListList* faceFacesPtr_;

    //- Edge-face addressing
    mutable labelListList* edgeFacesPtr_;

    //- Face-edge addressing
    mutable labelListList* faceEdgesPtr_;

    //- Point-edge addressing
    mutable labelListList* pointEdgesPtr_;

    //- Point-face addressing
    mutable labelListList* pointFacesPtr_;

    //- Faces addressing into local point list
    mutable List<Face>* localFacesPtr_;

    //- Labels of mesh points
    mutable labelList* meshPointsPtr_;

    //- Mesh point map.  Given the global point index find its
    //location in the patch
    mutable Map<label>* meshPointMapPtr_;

    //- Outside edge loops
    mutable labelListList* edgeLoopsPtr_;

    //- Points local to patch
    mutable Field<PointType>* localPointsPtr_;

    //- Local point order for most efficient search
    mutable labelList* localPointOrderPtr_;

    //- Face centres
    mutable Field<PointType>* faceCentresPtr_;

    //- Face unit normals
    mutable Field<PointType>* faceNormalsPtr_;

    //- Point unit normals
    mutable Field<PointType>* pointNormalsPtr_;

  // Private Member Functions

    //- Calculate edges of the patch
    void calcIntBdryEdges() const;

    //- Calculated boundary points on a patch
    void calcBdryPoints() const;

    //- Calculate addressing
    void calcAddressing() const;

    //- Calculate point-edge addressing
    void calcPointEdges() const;

    //- Calculate point-face addressing
    void calcPointFaces() const;

    //- Calculate mesh addressing
    void calcMeshData() const;

    //- Calculate mesh point map
    void calcMeshPointMap() const;

    //- Calculate outside edge loops
    void calcEdgeLoops() const;

    //- Calculate local points
    void calcLocalPoints() const;

    //- Calculate local point order
    void calcLocalPointOrder() const;

    //- Calculate face centres
    void calcFaceCentres() const;

    //- Calculate unit face normals
    void calcFaceNormals() const;

    //- Calculate unit point normals
    void calcPointNormals() const;

    //- Calculate edge owner
    void calcEdgeOwner() const;

    //- Face-edge-face walk while remaining on a patch point.
    //  Used to determine if surface multiply connected through point.
    void visitPointRegion
    (
      const label pointI,
      const labelList& pFaces,
      const label startFaceI,
      const label startEdgeI,
      boolList& pFacesHad
    ) const;

public:
  // Constructors

    //- Construct from components
    PrimitivePatch
    (
      const FaceList<Face>& faces,
      const Field<PointType>& points
    );

    //- Construct from components
    PrimitivePatch
    (
      const Xfer<FaceList<Face> >& faces,
      const Xfer<List<PointType> >& points
    );

    //- Construct from components, reuse storage
    PrimitivePatch
    (
      FaceList<Face>& faces,
      Field<PointType>& points,
      const bool reUse
    );

    //- Construct as copy
    PrimitivePatch
    (
      const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );

  //- Destructor
  virtual ~PrimitivePatch();
    void clearOut();
    void clearGeom();
    void clearTopology();
    void clearPatchMeshAddr();

  // Member Functions

  // Access
    //- Return reference to global points
    const Field<PointType>& points() const
    {
      return points_;
    }

  // Access functions for demand driven data

    // Topological data; no mesh required.

      //- Return number of points supporting patch faces
      label nPoints() const
      {
        return meshPoints().size();
      }

      //- Return number of edges in patch
      label nEdges() const
      {
        return edges().size();
      }

      //- Return list of edges, address into LOCAL point list
      const edgeList& edges() const;

      //- Number of internal edges
      label nInternalEdges() const;

      //- Is internal edge?
      bool isInternalEdge(const label edgeI) const
      {
        return edgeI < nInternalEdges();
      }

      //- Return list of boundary points,
      //  address into LOCAL point list
      const labelList& boundaryPoints() const;

      //- Return face-face addressing
      const labelListList& faceFaces() const;

      //- Return edge-face addressing
      const labelListList& edgeFaces() const;

      //- Return face-edge addressing
      const labelListList& faceEdges() const;

      //- Return point-edge addressing
      const labelListList& pointEdges() const;

      //- Return point-face addressing
      const labelListList& pointFaces() const;

      //- Return patch faces addressing into local point list
      const List<Face>& localFaces() const;

    // Addressing into mesh

      //- Return labelList of mesh points in patch. They are constructed
      //  walking through the faces in incremental order and not sorted
      //  anymore.
      const labelList& meshPoints() const;

      //- Mesh point map.  Given the global point index find its
      //  location in the patch
      const Map<label>& meshPointMap() const;

      //- Return pointField of points in patch
      const Field<PointType>& localPoints() const;

      //- Return orders the local points for most efficient search
      const labelList& localPointOrder() const;

      //- Given a global point index, return the local point index.
      //  If the point is not found, return -1
      label whichPoint(const label gp) const;

      //- Given an edge in local point labels, return its
      //  index in the edge list.  If the edge is not found, return -1
      label whichEdge(const edge&) const;

      //- Return labels of patch edges in the global edge list using
      //  cell addressing
      labelList meshEdges
      (
        const edgeList& allEdges,
        const labelListList& cellEdges,
        const labelList& faceCells
      ) const;

      //- Return labels of patch edges in the global edge list using
      //  basic edge addressing.
      labelList meshEdges
      (
        const edgeList& allEdges,
        const labelListList& pointEdges
      ) const;

      //- Return face centres for patch
      const Field<PointType>& faceCentres() const;

      //- Return face normals for patch
      const Field<PointType>& faceNormals() const;

      //- Return point normals for patch
      const Field<PointType>& pointNormals() const;

    // Other patch operations

      //- Project vertices of patch onto another patch
      template<class ToPatch>
      List<objectHit> projectPoints
      (
        const ToPatch& targetPatch,
        const Field<PointType>& projectionDirection,
        const intersection::algorithm = intersection::FULL_RAY,
        const intersection::direction = intersection::VECTOR
      ) const;

      //- Project vertices of patch onto another patch
      template<class ToPatch>
      List<objectHit> projectFaceCentres
      (
        const ToPatch& targetPatch,
        const Field<PointType>& projectionDirection,
        const intersection::algorithm = intersection::FULL_RAY,
        const intersection::direction = intersection::VECTOR
      ) const;

      //- Return list of closed loops of boundary vertices.
      //  Edge loops are given as ordered lists of vertices
      //  in local addressing
      const labelListList& edgeLoops() const;

  // Check

    //- Calculate surface type formed by patch.
    //  Types:
    //  - all edges have two neighbours (manifold)
    //  - some edges have more than two neighbours (illegal)
    //  - other (open)
    surfaceTopo surfaceType() const;

    //- Check surface formed by patch for manifoldness (see above).
    //  Return true if any incorrect edges are found.
    //  Insert vertices of incorrect edges into set.
    bool checkTopology
    (
      const bool report = false,
      labelHashSet* setPtr = NULL
    ) const;

    //- Checks primitivePatch for faces sharing point but not edge.
    //  This denotes a surface that is pinched at a single point
    //  (test for pinched at single edge is already in PrimitivePatch)
    //  Returns true if this situation found and puts conflicting
    //  (mesh)point in set. Based on all the checking routines in
    //  primitiveMesh.
    bool checkPointManifold
    (
      const bool report = false,
      labelHashSet* setPtr = NULL
    ) const;

  // Edit

    //- Correct patch after moving points
    virtual void movePoints(const Field<PointType>&);

  // Member operators
    //- Assignment
    void operator=
    (
      const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "_primitive_patch.cpp"
#endif
#endif

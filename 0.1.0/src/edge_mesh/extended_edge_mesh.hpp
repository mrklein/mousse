// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedEdgeMesh
// Description
//   Description of feature edges and points.
//   Feature points are a sorted subset at the start of the overall points list:
//     0 .. concaveStart_-1                : convex points (w.r.t normals)
//     concaveStart_ .. mixedStart_-1      : concave points
//     mixedStart_ .. nonFeatureStart_-1   : mixed internal/external points
//     nonFeatureStart_ .. size-1          : non-feature points
//   Feature edges are the edgeList of the edgeMesh and are sorted:
//     0 .. internalStart_-1           : external edges (convex w.r.t normals)
//     internalStart_ .. flatStart_-1  : internal edges (concave)
//     flatStart_ .. openStart_-1      : flat edges (neither concave or convex)
//                      can arise from region interfaces on
//                      flat surfaces
//     openStart_ .. multipleStart_-1  : open edges (e.g. from baffle surfaces)
//     multipleStart_ .. size-1        : multiply connected edges
//   The edge direction and feature edge and feature point adjacent normals
//   are stored.
// SourceFiles
//   extended_edge_mesh.cpp
//   extended_edge_mesh_new.cpp

#ifndef extended_edge_mesh_hpp_
#define extended_edge_mesh_hpp_

#include "edge_mesh.hpp"
#include "indexed_octree.hpp"
#include "tree_data_edge.hpp"
#include "tree_data_point.hpp"
#include "primitive_patch.hpp"
#include "ptr_list.hpp"
#include "unit_conversion.hpp"

namespace mousse
{

class surfaceFeatures;
class objectRegistry;
// Forward declaration of friend functions and operators
class extendedEdgeMesh;
Istream& operator>>(Istream&, extendedEdgeMesh&);
Ostream& operator<<(Ostream&, const extendedEdgeMesh&);

class extendedEdgeMesh
:
  public edgeMesh
{
public:
  //- Runtime type information
  TYPE_NAME("extendedEdgeMesh");

  enum pointStatus
  {
    CONVEX,     // Fully convex point (w.r.t normals)
    CONCAVE,    // Fully concave point
    MIXED,      // A point surrounded by both convex and concave edges
    NONFEATURE  // Not a feature point
  };

  static const mousse::NamedEnum<pointStatus, 4> pointStatusNames_;

  enum edgeStatus
  {
    EXTERNAL,   // "Convex" edge
    INTERNAL,   // "Concave" edge
    FLAT,       // Neither concave or convex, on a flat surface
    OPEN,       // i.e. only connected to one face
    MULTIPLE,   // Multiply connected (connected to more than two faces)
    NONE        // Not a classified feature edge (consistency with
          // surfaceFeatures)
  };

  static const mousse::NamedEnum<edgeStatus, 6> edgeStatusNames_;

  //- Normals point to the outside
  enum sideVolumeType
  {
    INSIDE  = 0,  // mesh inside
    OUTSIDE = 1,  // mesh outside
    BOTH    = 2,  // e.g. a baffle
    NEITHER = 3   // not sure when this may be used
  };

  static const mousse::NamedEnum<sideVolumeType, 4> sideVolumeTypeNames_;

  //- Angular closeness tolerance for treating normals as the same
  static scalar cosNormalAngleTol_;

protected:

  // Static data

    //- Index of the start of the convex feature points - static as 0
    static label convexStart_;

    //- Index of the start of the external feature edges - static as 0
    static label externalStart_;

  // Private data

    //- Index of the start of the concave feature points
    label concaveStart_;

    //- Index of the start of the mixed type feature points
    label mixedStart_;

    //- Index of the start of the non-feature points
    label nonFeatureStart_;

    //- Index of the start of the internal feature edges
    label internalStart_;

    //- Index of the start of the flat feature edges
    label flatStart_;

    //- Index of the start of the open feature edges
    label openStart_;

    //- Index of the start of the multiply-connected feature edges
    label multipleStart_;

    //- Normals of the features, to be referred to by index by both feature
    //  points and edges, unsorted
    vectorField normals_;

    //- Type per normal: which side of normal to mesh
    List<sideVolumeType> normalVolumeTypes_;

    //- Flat and open edges require the direction of the edge
    vectorField edgeDirections_;

    //- Starting directions for the edges.
    //  This vector points to the half of the plane defined by the first
    //  edge normal.
    labelListList normalDirections_;

    //- Indices of the normals that are adjacent to the feature edges
    labelListList edgeNormals_;

    //- Indices of the normals that are adjacent to the feature points
    //  (only valid for 0..nonFeatureStart_-1)
    labelListList featurePointNormals_;

    //- Indices of feature edges attached to feature points. The edges are
    //  ordered so that they can be circulated.
    labelListList featurePointEdges_;

    //- Feature edges which are on the boundary between regions
    labelList regionEdges_;

    //- Search tree for all feature points
    mutable autoPtr<indexedOctree<treeDataPoint> > pointTree_;

    //- Search tree for all edges
    mutable autoPtr<indexedOctree<treeDataEdge> > edgeTree_;

    //- Individual search trees for each type of edge
    mutable PtrList<indexedOctree<treeDataEdge> > edgeTreesByType_;

  // Private Member Functions

    //- Classify the type of feature point.  Requires valid stored member
    //  data for edges and normals.
    pointStatus classifyFeaturePoint(label ptI) const;

    template<class Patch>
    void sortPointsAndEdges
    (
      const Patch&,
      const labelList& featureEdges,
      const labelList& regionFeatureEdges,
      const labelList& feaurePoints
    );

public:

  // Static data

    //- Number of possible point types (i.e. number of slices)
    static label nPointTypes;

    //- Number of possible feature edge types (i.e. number of slices)
    static label nEdgeTypes;

    //- Can we read this file format?
    static bool canRead(const fileName&, const bool verbose=false);

    //- Can we read this file format?
    static bool canReadType(const word& ext, const bool verbose=false);

    //- Can we write this file format type?
    static bool canWriteType(const word& ext, const bool verbose=false);

    static wordHashSet readTypes();

    static wordHashSet writeTypes();

  // Constructors

    //- Construct null
    extendedEdgeMesh();

    //- Construct as copy
    explicit extendedEdgeMesh(const extendedEdgeMesh&);

    //- Construct from file name (uses extension to determine type)
    extendedEdgeMesh(const fileName&);

    //- Construct from file name (uses extension to determine type)
    extendedEdgeMesh(const fileName&, const word& ext);

    //- Construct from Istream
    extendedEdgeMesh(Istream&);

    //- Construct by transferring components (points, edges)
    extendedEdgeMesh
    (
      const Xfer<pointField>&,
      const Xfer<edgeList>&
    );

    //- Construct given a surface with selected edges,point
    //  (surfaceFeatures), an objectRegistry and a
    //  fileName to write to.
    //  Extracts, classifies and reorders the data from surfaceFeatures.
    extendedEdgeMesh
    (
      const surfaceFeatures& sFeat,
      const boolList& surfBaffleRegions
    );

    //- Construct from PrimitivePatch
    extendedEdgeMesh
    (
      const PrimitivePatch<face, List, pointField, point>& surf,
      const labelList& featureEdges,
      const labelList& regionFeatureEdges,
      const labelList& featurePoints
    );

    //- Construct from all components
    extendedEdgeMesh
    (
      const pointField& pts,
      const edgeList& eds,
      label concaveStart,
      label mixedStart,
      label nonFeatureStart,
      label internalStart,
      label flatStart,
      label openStart,
      label multipleStart,
      const vectorField& normals,
      const List<sideVolumeType>& normalVolumeTypes,
      const vectorField& edgeDirections,
      const labelListList& normalDirections,
      const labelListList& edgeNormals,
      const labelListList& featurePointNormals,
      const labelListList& featurePointEdges,
      const labelList& regionEdges
    );

  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      extendedEdgeMesh,
      fileExtension,
      (
        const fileName& name
      ),
      (name)
    );

  // Selectors

    //- Select constructed from filename (explicit extension)
    static autoPtr<extendedEdgeMesh> New
    (
      const fileName&,
      const word& ext
    );

    //- Select constructed from filename (implicit extension)
    static autoPtr<extendedEdgeMesh> New(const fileName&);

  //- Destructor
  ~extendedEdgeMesh();

  // Member Functions

    // Find

      //- Find nearest surface edge for the sample point.
      void nearestFeaturePoint
      (
        const point& sample,
        scalar searchDistSqr,
        pointIndexHit& info
      ) const;

      //- Find nearest surface edge for the sample point.
      void nearestFeatureEdge
      (
        const point& sample,
        scalar searchDistSqr,
        pointIndexHit& info
      ) const;

      //- Find nearest surface edge for each sample point.
      void nearestFeatureEdge
      (
        const pointField& samples,
        const scalarField& searchDistSqr,
        List<pointIndexHit>& info
      ) const;

      //- Find the nearest point on each type of feature edge
      void nearestFeatureEdgeByType
      (
        const point& sample,
        const scalarField& searchDistSqr,
        List<pointIndexHit>& info
      ) const;

      //- Find all the feature points within searchDistSqr of sample
      void allNearestFeaturePoints
      (
        const point& sample,
        scalar searchRadiusSqr,
        List<pointIndexHit>& info
      ) const;

      //- Find all the feature edges within searchDistSqr of sample
      void allNearestFeatureEdges
      (
        const point& sample,
        const scalar searchRadiusSqr,
        List<pointIndexHit>& info
      ) const;

    // Access

      //- Return the index of the start of the convex feature points
      inline label convexStart() const;

      //- Return the index of the start of the concave feature points
      inline label concaveStart() const;

      //- Return the index of the start of the mixed type feature points
      inline label mixedStart() const;

      //- Return the index of the start of the non-feature points
      inline label nonFeatureStart() const;

      //- Return the index of the start of the external feature edges
      inline label externalStart() const;

      //- Return the index of the start of the internal feature edges
      inline label internalStart() const;

      //- Return the index of the start of the flat feature edges
      inline label flatStart() const;

      //- Return the index of the start of the open feature edges
      inline label openStart() const;

      //- Return the index of the start of the multiply-connected feature
      //  edges
      inline label multipleStart() const;

      //- Return whether or not the point index is a feature point
      inline bool featurePoint(label ptI) const;

      //- Return the normals of the surfaces adjacent to the feature edges
      //  and points
      inline const vectorField& normals() const;

      //- Return
      inline const List<sideVolumeType>& normalVolumeTypes() const;

      //- Return the edgeDirection vectors
      inline const vectorField& edgeDirections() const;

      //-
      inline const labelListList& normalDirections() const;

      //- Return the direction of edgeI, pointing away from ptI
      inline vector edgeDirection(label edgeI, label ptI) const;

      //- Return the indices of the normals that are adjacent to the
      //  feature edges
      inline const labelListList& edgeNormals() const;

      //- Return the normal vectors for a given set of normal indices
      inline vectorField edgeNormals(const labelList& edgeNormIs) const;

      //- Return the normal vectors for a given edge
      inline vectorField edgeNormals(label edgeI) const;

      //- Return the indices of the normals that are adjacent to the
      //  feature points
      inline const labelListList& featurePointNormals() const;

      //- Return the normal vectors for a given feature point
      inline vectorField featurePointNormals(label ptI) const;

      //- Return the edge labels for a given feature point. Edges are
      //  ordered by the faces that they share. The edge labels
      //  correspond to the entry in edges().
      inline const labelListList& featurePointEdges() const;

      //- Return the feature edges which are on the boundary between
      //  regions
      inline const labelList& regionEdges() const;

      //- Return the pointStatus of a specified point
      inline pointStatus getPointStatus(label ptI) const;

      //- Return the edgeStatus of a specified edge
      inline edgeStatus getEdgeStatus(label edgeI) const;

      //- Return the baffle faces of a specified edge
      inline PackedList<2> edgeBaffles(label edgeI) const;

      //- Demand driven construction of octree for feature points
      const indexedOctree<treeDataPoint>& pointTree() const;

      //- Demand driven construction of octree for boundary edges
      const indexedOctree<treeDataEdge>& edgeTree() const;

      //- Demand driven construction of octree for boundary edges by type
      const PtrList<indexedOctree<treeDataEdge> >&
      edgeTreesByType() const;

    // Edit

      //- Transfer the contents of the argument and annul the argument
      void transfer(extendedEdgeMesh&);

      //- Transfer contents to the Xfer container
      Xfer<extendedEdgeMesh > xfer();

      //- Clear all storage
      virtual void clear();

      //- Add extendedEdgeMesh. No filtering of duplicates.
      void add(const extendedEdgeMesh&);

      //- Flip normals. All concave become convex, all internal external
      //  etc.
      void flipNormals();

    // Read

      //- Read from file. Chooses reader based on explicit extension
      bool read(const fileName&, const word& ext);

      //- Read from file. Chooses reader based on detected extension
      virtual bool read(const fileName&);

    // Write

      //- Write all components of the extendedEdgeMesh as obj files
      void writeObj(const fileName& prefix) const;

      //- Dump some information
      virtual void writeStats(Ostream& os) const;
      friend Istream& operator>>(Istream& is, sideVolumeType& vt);
      friend Ostream& operator<<(Ostream& os, const sideVolumeType& vt);

    //- Classify the type of feature edge.  Requires face centre 0 to face
    //  centre 1 vector to distinguish internal from external
    static edgeStatus classifyEdge
    (
      const List<vector>& norms,
      const labelList& edNorms,
      const vector& fC0tofC1
    );

    // Ostream Operator
      friend Ostream& operator<<(Ostream&, const extendedEdgeMesh&);
      friend Istream& operator>>(Istream&, extendedEdgeMesh&);
};

}  // namespace mousse

// Member Functions 
inline mousse::label mousse::extendedEdgeMesh::convexStart() const
{
  return convexStart_;
}

inline mousse::label mousse::extendedEdgeMesh::concaveStart() const
{
  return concaveStart_;
}

inline mousse::label mousse::extendedEdgeMesh::mixedStart() const
{
  return mixedStart_;
}

inline mousse::label mousse::extendedEdgeMesh::nonFeatureStart() const
{
  return nonFeatureStart_;
}

inline mousse::label mousse::extendedEdgeMesh::externalStart() const
{
  return externalStart_;
}

inline mousse::label mousse::extendedEdgeMesh::internalStart() const
{
  return internalStart_;
}

inline mousse::label mousse::extendedEdgeMesh::flatStart() const
{
  return flatStart_;
}

inline mousse::label mousse::extendedEdgeMesh::openStart() const
{
  return openStart_;
}

inline mousse::label mousse::extendedEdgeMesh::multipleStart() const
{
  return multipleStart_;
}

inline bool mousse::extendedEdgeMesh::featurePoint(label ptI) const
{
  return ptI < nonFeatureStart_;
}

inline const mousse::vectorField& mousse::extendedEdgeMesh::normals() const
{
  return normals_;
}

inline const mousse::List<mousse::extendedEdgeMesh::sideVolumeType>&
mousse::extendedEdgeMesh::normalVolumeTypes() const
{
  return normalVolumeTypes_;
}

inline const mousse::vectorField& mousse::extendedEdgeMesh::edgeDirections()
const
{
  return edgeDirections_;
}

inline const mousse::labelListList&
mousse::extendedEdgeMesh::normalDirections() const
{
  return normalDirections_;
}

inline mousse::vector mousse::extendedEdgeMesh::edgeDirection
(
  label edgeI,
  label ptI
) const
{
  const edge& e = edges()[edgeI];
  if (ptI == e.start())
  {
    return edgeDirections()[edgeI];
  }
  else if (ptI == e.end())
  {
    return -edgeDirections()[edgeI];
  }
  else
  {
    FATAL_ERROR_IN("mousse::extendedEdgeMesh::edgeDirection")
      << "Requested ptI " << ptI << " is not a point on the requested "
      << "edgeI " << edgeI << ". edgeI start and end: "
      << e.start() << " " << e.end()
      << exit(FatalError);
    return vector::zero;
  }
}

inline const mousse::labelListList& mousse::extendedEdgeMesh::edgeNormals()
const
{
  return edgeNormals_;
}

inline mousse::vectorField mousse::extendedEdgeMesh::edgeNormals
(
  const labelList& edgeNormIs
) const
{
  vectorField norms(edgeNormIs.size());
  FOR_ALL(edgeNormIs, i)
  {
    norms[i] = normals_[edgeNormIs[i]];
  }
  return norms;
}

inline mousse::vectorField mousse::extendedEdgeMesh::edgeNormals(label edgeI)
const
{
  return edgeNormals(edgeNormals_[edgeI]);
}

inline const mousse::labelListList&
mousse::extendedEdgeMesh::featurePointNormals() const
{
  return featurePointNormals_;
}

inline mousse::vectorField mousse::extendedEdgeMesh::featurePointNormals
(
  label ptI
) const
{
  if (!featurePoint(ptI))
  {
    WARNING_IN("vectorField extendedEdgeMesh::featurePointNormals")
      << "Requesting the normals of a non-feature point. "
      << "Returned zero length vectorField."
      << endl;
    return vectorField(0);
  }
  labelList featPtNormIs(featurePointNormals_[ptI]);
  vectorField norms(featPtNormIs.size());
  FOR_ALL(featPtNormIs, i)
  {
    norms[i] = normals_[featPtNormIs[i]];
  }
  return norms;
}

inline const mousse::labelListList&
mousse::extendedEdgeMesh::featurePointEdges() const
{
  return featurePointEdges_;
}

inline const mousse::labelList& mousse::extendedEdgeMesh::regionEdges() const
{
  return regionEdges_;
}

inline mousse::extendedEdgeMesh::pointStatus
mousse::extendedEdgeMesh::getPointStatus(label ptI) const
{
  if (ptI < concaveStart_)
  {
    return CONVEX;
  }
  else if (ptI < mixedStart_)
  {
    return CONCAVE;
  }
  else if (ptI < nonFeatureStart_)
  {
    return MIXED;
  }
  else
  {
    return NONFEATURE;
  }
}

inline mousse::extendedEdgeMesh::edgeStatus
mousse::extendedEdgeMesh::getEdgeStatus(label edgeI) const
{
  if (edgeI < internalStart_)
  {
    return EXTERNAL;
  }
  else if (edgeI < flatStart_)
  {
    return INTERNAL;
  }
  else if (edgeI < openStart_)
  {
    return FLAT;
  }
  else if (edgeI < multipleStart_)
  {
    return OPEN;
  }
  else
  {
    return MULTIPLE;
  }
}

inline mousse::PackedList<2> mousse::extendedEdgeMesh::edgeBaffles
(
  label edgeI
) const
{
  const labelList& eNormals = edgeNormals_[edgeI];
  DynamicList<label> edgeBaffles(eNormals.size());
  FOR_ALL(eNormals, enI)
  {
    const label normI = eNormals[enI];
    if (normalVolumeTypes_[normI])
    {
      edgeBaffles.append(normI);
    }
  }
  return PackedList<2>(edgeBaffles);
}

#ifdef NoRepository
#   include "extended_edge_mesh_templates.cpp"
#endif
#endif

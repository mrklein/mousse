#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_FEATURE_POINT_CONFORMER_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_FEATURE_POINT_CONFORMER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::featurePointConformer
// Description
//   The Delaunay vertices required to conform to a feature point can be
//   determined upon initialisation because the feature points are fixed and
//   do not change throughout the meshing process.
// SourceFiles
//   feature_point_conformer.cpp
//   feature_point_conformer_specialisations.cpp
// Include uint.H before CGAL headers to define __STDC_LIMIT_MACROS
#include "uint.hpp"
#include "cgal_triangulation_3d_defs.hpp"
#include "vector.hpp"
#include "dynamic_list.hpp"
#include "list.hpp"
#include "extended_feature_edge_mesh.hpp"
#include "point_pairs.hpp"
namespace mousse
{
class conformalVoronoiMesh;
class cvControls;
class conformationSurfaces;
class pointFeatureEdgesTypes;
class backgroundMeshDecomposition;
class featurePointConformer
{
  // Static data
    //- Tolerance within which two lines are said to be parallel.
    static const scalar tolParallel;
  // Private data
    //- Reference to the mesher.
    const conformalVoronoiMesh& foamyHexMesh_;
    //- Reference to the mesher controls.
    const cvControls& foamyHexMeshControls_;
    //- Reference to the conformation surfaces.
    const conformationSurfaces& geometryToConformTo_;
    //- Store the feature constraining points, to be reinserted after a
    //  triangulation clear.
    List<Vb> featurePointVertices_;
    //-
    mutable pointPairs<Delaunay> ftPtPairs_;
  // Private Member Functions
    //- Calculate the shared face normal between two edges geometrically.
    vector sharedFaceNormal
    (
      const extendedFeatureEdgeMesh& feMesh,
      const label edgeI,
      const label nextEdgeI
    ) const;
    label getSign(const extendedFeatureEdgeMesh::edgeStatus eStatus) const;
    bool createSpecialisedFeaturePoint
    (
      const extendedFeatureEdgeMesh& feMesh,
      const labelList& pEds,
      const pointFeatureEdgesTypes& pFEdgesTypes,
      const List<extendedFeatureEdgeMesh::edgeStatus>& allEdStat,
      const label ptI,
      DynamicList<Vb>& pts
    ) const;
    void addMasterAndSlavePoints
    (
      const DynamicList<point>& masterPoints,
      const DynamicList<indexedVertexEnum::vertexType>& masterPointsTypes,
      const Map<DynamicList<autoPtr<plane> > >& masterPointReflections,
      DynamicList<Vb>& pts,
      const label ptI
    ) const;
    //- Helper function for conforming to feature points
    void createMasterAndSlavePoints
    (
      const extendedFeatureEdgeMesh& feMesh,
      const label ptI,
      DynamicList<Vb>& pts
    ) const;
    void createMixedFeaturePoints(DynamicList<Vb>& pts) const;
    //- Create the points that will conform to the feature
    void createFeaturePoints(DynamicList<Vb>& pts);
public:
  //- Runtime type information
  CLASS_NAME("featurePointConformer");
  // Constructors
    //- Construct from components
    explicit featurePointConformer
    (
      const conformalVoronoiMesh& foamyHexMesh
    );
    //- Disallow default bitwise copy construct
    featurePointConformer(const featurePointConformer&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const featurePointConformer&) = delete;
  //- Destructor
  ~featurePointConformer();
  // Member Functions
    // Access
      //- Return the feature point vertices for insertion into the
      //  triangulation.
      inline const List<Vb>& featurePointVertices() const;
      //- Return the feature point pair table
      inline const pointPairs<Delaunay>& featurePointPairs() const;
    // Edit
      //- Distribute the feature point vertices according to the
      //  supplied background mesh
      void distribute(const backgroundMeshDecomposition& decomposition);
      //- Reindex the feature point pairs using the map.
      void reIndexPointPairs(const Map<label>& oldToNewIndices);
};
}  // namespace mousse

const mousse::List<Vb>& mousse::featurePointConformer::featurePointVertices() const
{
  return featurePointVertices_;
}

const mousse::pointPairs<Delaunay>&
mousse::featurePointConformer::featurePointPairs() const
{
  return ftPtPairs_;
}
#endif

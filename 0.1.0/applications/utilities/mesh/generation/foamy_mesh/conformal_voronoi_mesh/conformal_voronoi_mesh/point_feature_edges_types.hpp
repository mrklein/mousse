#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_POINT_FEATURE_EDGES_TYPES_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_POINT_FEATURE_EDGES_TYPES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointFeatureEdgesTypes
// Description
//   Holds information on the types of feature edges attached to feature points.
// SourceFiles
//   point_feature_edges_types.cpp
#include "hash_table.hpp"
#include "extended_feature_edge_mesh.hpp"
#include "list.hpp"
namespace mousse
{
//- Hold the types of feature edges attached to the point.
class pointFeatureEdgesTypes
:
  public HashTable<label, extendedFeatureEdgeMesh::edgeStatus>
{
  // Private data
    //- Reference to the feature edge mesh
    const extendedFeatureEdgeMesh& feMesh_;
    //- Label of the point
    label pointLabel_;
public:
  // Constructors
    //- Construct from components
    pointFeatureEdgesTypes
    (
      const extendedFeatureEdgeMesh& feMesh,
      const label pointLabel
    );
  //- Destructor
  ~pointFeatureEdgesTypes();
  // Member Functions
    //- Fill the pointFeatureEdgesType class with the types of feature
    //  edges that are attached to the point.
    List<extendedFeatureEdgeMesh::edgeStatus> calcPointFeatureEdgesTypes();
  // Info
    friend Ostream& operator<<
    (
      Ostream& os,
      const pointFeatureEdgesTypes& p
    );
};
}  // namespace mousse
#endif

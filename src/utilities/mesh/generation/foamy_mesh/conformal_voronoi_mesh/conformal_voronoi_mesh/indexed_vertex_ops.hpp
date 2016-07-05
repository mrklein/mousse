#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_INDEXED_VERTEX_OPS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_INDEXED_VERTEX_OPS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"


namespace CGAL {
namespace indexedVertexOps {

//- Return the target cell size from that stored on a pair of Delaunay vertices,
//  using a mean function.
template<typename VertexType>
mousse::scalar averageCellSize(const VertexType& vA, const VertexType& vB);

template<typename VertexType>
inline bool uninitialised(const VertexType& v);

}  // namespace indexedVertexOps
}  // namespace CGAL

#include "indexed_vertex_ops.ipp"

#endif

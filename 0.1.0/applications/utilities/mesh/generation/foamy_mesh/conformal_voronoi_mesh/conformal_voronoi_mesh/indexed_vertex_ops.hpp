// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   indexed_vertex_ops_templates.cpp
#ifndef indexed_vertex_ops_hpp_
#define indexed_vertex_ops_hpp_
#include "scalar.hpp"
namespace CGAL
{
namespace indexedVertexOps
{
//- Return the target cell size from that stored on a pair of Delaunay vertices,
//  using a mean function.
template<typename VertexType>
mousse::scalar averageCellSize(const VertexType& vA, const VertexType& vB);
template<typename VertexType>
inline bool uninitialised(const VertexType& v);
}  // namespace indexedVertexOps
}  // namespace CGAL
#ifdef NoRepository
  #include "indexed_vertex_ops_templates.cpp"
#endif
#endif

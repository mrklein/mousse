// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_vertex_ops.hpp"
// Global Functions 
template<typename VertexType>
mousse::scalar CGAL::indexedVertexOps::averageCellSize
(
  const VertexType& vA,
  const VertexType& vB
)
{
  // Arithmetic mean
  // return 0.5*(vA->targetCellSize() + vB->targetCellSize());
  // Geometric mean
  return sqrt(vA->targetCellSize()*vB->targetCellSize());
  // Harmonic mean
  // return
  //     2.0*(vA->targetCellSize()*vB->targetCellSize())
  //    /(vA->targetCellSize() + vB->targetCellSize());
}
template<typename VertexType>
inline bool CGAL::indexedVertexOps::uninitialised(const VertexType& v)
{
  return v->type() == mousse::indexedVertexEnum::vtUnassigned;
}

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
mousse::cellShapeControlMesh::Vertex_handle mousse::cellShapeControlMesh::insert
(
  const mousse::point& pt,
  const scalar& size,
  const triad& alignment,
  const mousse::indexedVertexEnum::vertexType type
)
{
  Vertex_handle v = CellSizeDelaunay::insert
  (
    Point(pt.x(), pt.y(), pt.z())
  );
  v->type() = type;
  v->index() = getNewVertexIndex();
  v->procIndex() = Pstream::myProcNo();
  v->targetCellSize() = size;
  v->alignment() = tensor(alignment.x(), alignment.y(), alignment.z());
  return v;
}
mousse::cellShapeControlMesh::Vertex_handle mousse::cellShapeControlMesh::insertFar
(
  const mousse::point& pt
)
{
  Vertex_handle v = CellSizeDelaunay::insert
  (
    Point(pt.x(), pt.y(), pt.z())
  );
  v->type() = Vb::vtFar;
//    v->type() = Vb::vtExternalFeaturePoint;
  v->index() = getNewVertexIndex();
  v->procIndex() = Pstream::myProcNo();
  return v;
}

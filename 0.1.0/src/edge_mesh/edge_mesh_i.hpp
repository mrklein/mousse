// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::edgeMesh::edgeMesh(const edgeMesh& em)
:
  fileFormats::edgeMeshFormatsCore(),
  points_(em.points_),
  edges_(em.edges_),
  pointEdgesPtr_(NULL)
{}
// Member Functions 
inline const mousse::pointField& mousse::edgeMesh::points() const
{
  return points_;
}
inline const mousse::edgeList& mousse::edgeMesh::edges() const
{
  return edges_;
}
inline const mousse::labelListList& mousse::edgeMesh::pointEdges() const
{
  if (pointEdgesPtr_.empty())
  {
    calcPointEdges();
  }
  return pointEdgesPtr_();
}
inline mousse::pointField& mousse::edgeMesh::storedPoints()
{
  return points_;
}
inline mousse::edgeList& mousse::edgeMesh::storedEdges()
{
  return edges_;
}
// Member Operators 
void mousse::edgeMesh::operator=(const edgeMesh& rhs)
{
  points_ = rhs.points_;
  edges_ = rhs.edges_;
  pointEdgesPtr_.clear();
}

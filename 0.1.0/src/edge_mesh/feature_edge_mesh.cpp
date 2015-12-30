// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "feature_edge_mesh.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(featureEdgeMesh, 0);
}
// Constructors 
mousse::featureEdgeMesh::featureEdgeMesh(const IOobject& io)
:
  regIOobject(io),
  edgeMesh(pointField(0), edgeList(0))
{
  if
  (
    io.readOpt() == IOobject::MUST_READ
  || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
  || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    readStream(typeName) >> *this;
    close();
  }
  if (debug)
  {
    Pout<< "featureEdgeMesh::featureEdgeMesh :"
      << " constructed from IOobject :"
      << " points:" << points().size()
      << " edges:" << edges().size()
      << endl;
  }
}
//- Construct from components
mousse::featureEdgeMesh::featureEdgeMesh
(
  const IOobject& io,
  const pointField& points,
  const edgeList& edges
)
:
  regIOobject(io),
  edgeMesh(points, edges)
{}
// Construct as copy
mousse::featureEdgeMesh::featureEdgeMesh
(
  const IOobject& io,
  const featureEdgeMesh& em
)
:
  regIOobject(io),
  edgeMesh(em)
{}
// Member Functions 
bool mousse::featureEdgeMesh::readData(Istream& is)
{
  is >> *this;
  return !is.bad();
}
bool mousse::featureEdgeMesh::writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}

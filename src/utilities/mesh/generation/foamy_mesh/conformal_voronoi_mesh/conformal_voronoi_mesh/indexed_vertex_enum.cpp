// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_vertex_enum.hpp"
#include "pstream.hpp"


// Static Data Members
template<>
const char*
mousse::NamedEnum<mousse::indexedVertexEnum::vertexType, 15>::names[] =
{
  "Unassigned",
  "Internal",
  "InternalNearBoundary",
  "InternalSurface",
  "InternalSurfaceBaffle",
  "ExternalSurfaceBaffle",
  "InternalFeatureEdge",
  "InternalFeatureEdgeBaffle",
  "ExternalFeatureEdgeBaffle",
  "InternalFeaturePoint",
  "ExternalSurface",
  "ExternalFeatureEdge",
  "ExternalFeaturePoint",
  "Far",
  "Constrained"
};

const mousse::NamedEnum<mousse::indexedVertexEnum::vertexType, 15>
mousse::indexedVertexEnum::vertexTypeNames_;


template<>
const char*
mousse::NamedEnum<mousse::indexedVertexEnum::vertexMotion, 2>::names[] =
{
  "fixed",
  "movable"
};


const mousse::NamedEnum<mousse::indexedVertexEnum::vertexMotion, 2>
vertexMotionNames_;
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const mousse::indexedVertexEnum::vertexType& v
)
{
  os << static_cast<int>(v);
  return os;
}


mousse::Istream& mousse::operator>>
(
  Istream& is,
  mousse::indexedVertexEnum::vertexType& v
)
{
  int type;
  is >> type;
  v = static_cast<mousse::indexedVertexEnum::vertexType>(type);
  return is;
}


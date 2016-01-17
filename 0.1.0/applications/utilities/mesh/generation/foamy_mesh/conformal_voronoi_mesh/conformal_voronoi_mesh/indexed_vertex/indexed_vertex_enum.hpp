// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedVertexEnum
// Description
// SourceFiles
//   indexed_vertex_enum.cpp
#ifndef indexed_vertex_enum_hpp_
#define indexed_vertex_enum_hpp_
#include "named_enum.hpp"
namespace mousse
{
class indexedVertexEnum
{
public:
  enum vertexType
  {
    vtUnassigned                = 0,
    vtInternal                  = 1,
    vtInternalNearBoundary      = 2,
    vtInternalSurface           = 3,
    vtInternalSurfaceBaffle     = 4,
    vtExternalSurfaceBaffle     = 5,
    vtInternalFeatureEdge       = 6,
    vtInternalFeatureEdgeBaffle = 7,
    vtExternalFeatureEdgeBaffle = 8,
    vtInternalFeaturePoint      = 9,
    vtExternalSurface           = 10,
    vtExternalFeatureEdge       = 11,
    vtExternalFeaturePoint      = 12,
    vtFar                       = 13,
    vtConstrained               = 14
  };
  enum vertexMotion
  {
    fixed   = 0,
    movable = 1
  };
  static const mousse::NamedEnum<vertexType, 15> vertexTypeNames_;
  static const mousse::NamedEnum<vertexMotion, 2> vertexMotionNames_;
  friend Ostream& operator<<(mousse::Ostream&, const vertexType&);
  friend Istream& operator>>(mousse::Istream&, vertexType&);
};
template<>
inline bool contiguous<indexedVertexEnum>()
{
  return true;
}
}  // namespace mousse
#endif

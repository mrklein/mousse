// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_location.hpp"
#include "tri_surface.hpp"
// Member Functions 
mousse::vector mousse::surfaceLocation::normal(const triSurface& s) const
{
  const vectorField& n = s.faceNormals();
  if (elementType_ == triPointRef::NONE)
  {
    return n[index()];
  }
  else if (elementType_ == triPointRef::EDGE)
  {
    const labelList& eFaces = s.edgeFaces()[index()];
    if (eFaces.size() == 1)
    {
      return n[eFaces[0]];
    }
    else
    {
      vector edgeNormal(vector::zero);
      forAll(eFaces, i)
      {
        edgeNormal += n[eFaces[i]];
      }
      return edgeNormal/(mag(edgeNormal) + VSMALL);
    }
  }
  else
  {
    return s.pointNormals()[index()];
  }
}
// Friend Operators 
void mousse::surfaceLocation::write(Ostream& os, const triSurface& s) const
{
  if (elementType_ == triPointRef::NONE)
  {
    os  << "trianglecoords:" << s[index()].tri(s.points());
  }
  else if (elementType() == triPointRef::EDGE)
  {
    const edge& e = s.edges()[index()];
    os  << "edgecoords:" << e.line(s.localPoints());
  }
  else
  {
    os  << "pointcoord:" << s.localPoints()[index()];
  }
}
mousse::Istream& mousse::operator>>(Istream& is, surfaceLocation& sl)
{
  label elType;
  is  >> static_cast<pointIndexHit&>(sl)
    >> elType >> sl.triangle_;
  sl.elementType_ = triPointRef::proxType(elType);
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const surfaceLocation& sl)
{
  return os
    << static_cast<const pointIndexHit&>(sl)
    << token::SPACE << label(sl.elementType_)
    << token::SPACE << sl.triangle_;
}
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const InfoProxy<surfaceLocation>& ip
)
{
  const surfaceLocation& sl = ip.t_;
  if (sl.elementType() == triPointRef::NONE)
  {
    os  << "coord:" << sl.rawPoint()
      << " inside triangle:" << sl.index()
      << " excludeTriangle:" << sl.triangle();
  }
  else if (sl.elementType() == triPointRef::EDGE)
  {
    os  << "coord:" << sl.rawPoint()
      << " on edge:" << sl.index()
      << " excludeTriangle:" << sl.triangle();
  }
  else
  {
    os  << "coord:" << sl.rawPoint()
      << " on point:" << sl.index()
      << " excludeTriangle:" << sl.triangle();
  }
  if (sl.hit())
  {
    os  << " (hit)";
  }
  else
  {
    os  << " (miss)";
  }
  return os;
}

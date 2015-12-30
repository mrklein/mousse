// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_normal_info.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::wallNormalInfo& wDist
)
{
  if (os.format() == IOstream::ASCII)
  {
    os << wDist.normal();
  }
  else
  {
    os.write
    (
      reinterpret_cast<const char*>(&wDist.normal_),
      sizeof(vector)
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const wallNormalInfo&)");
  return os;
}
mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::wallNormalInfo& wDist)
{
  if (is.format() == IOstream::ASCII)
  {
    is >> wDist.normal_;
  }
  else
  {
    is.read
    (
      reinterpret_cast<char*>(&wDist.normal_),
      sizeof(vector)
    );
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, wallNormalInfo&)");
  return is;
}

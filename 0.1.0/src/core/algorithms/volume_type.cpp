// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "volume_type.hpp"
#include "istream.hpp"

// Static Data Members
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::volumeType,
    4
  >::names[] =
  {
    "unknown",
    "mixed",
    "inside",
    "outside"
  };
}
const mousse::NamedEnum<mousse::volumeType, 4> mousse::volumeType::names;

// IOstream Operators
mousse::Istream& mousse::operator>>(Istream& is, volumeType& vt)
{
  // Read beginning of volumeType
  is.readBegin("volumeType");
  int type;
  is  >> type;
  vt.t_ = static_cast<volumeType::type>(type);
  // Read end of volumeType
  is.readEnd("volumeType");
  // Check state of Istream
  is.check("operator>>(Istream&, volumeType&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const volumeType& vt)
{
  os  << static_cast<int>(vt.t_);
  return os;
}

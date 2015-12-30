// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_zone.hpp"
#include "dictionary.hpp"
#include "word.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(surfZone, 0);
}
// Constructors 
mousse::surfZone::surfZone()
:
  surfZoneIdentifier(),
  size_(0),
  start_(0)
{}
mousse::surfZone::surfZone
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const word& geometricType
)
:
  surfZoneIdentifier(name, index, geometricType),
  size_(size),
  start_(start)
{}
mousse::surfZone::surfZone(Istream& is, const label index)
:
  surfZoneIdentifier(),
  size_(0),
  start_(0)
{
  word name(is);
  dictionary dict(is);
  operator=(surfZone(name, dict, index));
}
mousse::surfZone::surfZone
(
  const word& name,
  const dictionary& dict,
  const label index
)
:
  surfZoneIdentifier(name, dict, index),
  size_(readLabel(dict.lookup("nFaces"))),
  start_(readLabel(dict.lookup("startFace")))
{}
mousse::surfZone::surfZone(const surfZone& zone)
:
  surfZoneIdentifier(zone, zone.index()),
  size_(zone.size()),
  start_(zone.start())
{}
mousse::surfZone::surfZone(const surfZone& zone, const label index)
:
  surfZoneIdentifier(zone, index),
  size_(zone.size()),
  start_(zone.start())
{}
// Member Functions 
void mousse::surfZone::write(Ostream& os) const
{
  writeDict(os);
}
void mousse::surfZone::writeDict(Ostream& os) const
{
  os  << indent << name() << nl
    << indent << token::BEGIN_BLOCK << incrIndent << nl;
  surfZoneIdentifier::write(os);
  os.writeKeyword("nFaces") << size() << token::END_STATEMENT << nl;
  os.writeKeyword("startFace") << start() << token::END_STATEMENT << nl;
  os  << decrIndent << indent << token::END_BLOCK << endl;
}
// Member Operators 
bool mousse::surfZone::operator!=(const surfZone& rhs) const
{
  return !(*this == rhs);
}
bool mousse::surfZone::operator==(const surfZone& rhs) const
{
  return
  (
    size() == rhs.size()
  && start() == rhs.start()
  && geometricType() == rhs.geometricType()
  );
}
// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, surfZone& zone)
{
  zone = surfZone(is, 0);
  is.check("Istream& operator>>(Istream&, surfZone&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const surfZone& zone)
{
  zone.write(os);
  os.check("Ostream& operator<<(Ostream&, const surfZone&");
  return os;
}

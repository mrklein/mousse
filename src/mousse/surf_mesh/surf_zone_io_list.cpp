// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_zone_io_list.hpp"
#include "ptr_list.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(surfZoneIOList, 0);

}


// Constructors
mousse::surfZoneIOList::surfZoneIOList
(
  const IOobject& io
)
:
  surfZoneList{},
  regIOobject{io}
{
  mousse::string functionName =
    "surfZoneIOList::surfZoneIOList"
    "(const IOobject& io)";
  if (readOpt() == IOobject::MUST_READ
      || readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    surfZoneList& zones = *this;
    Istream& is = readStream(typeName);
    PtrList<entry> dictEntries{is};
    zones.setSize(dictEntries.size());
    label faceI = 0;
    FOR_ALL(zones, zoneI) {
      const dictionary& dict = dictEntries[zoneI].dict();
      label zoneSize = readLabel(dict.lookup("nFaces"));
      label startFaceI = readLabel(dict.lookup("startFace"));
      zones[zoneI] = surfZone{dictEntries[zoneI].keyword(), zoneSize,
        startFaceI, zoneI};
      word geoType;
      if (dict.readIfPresent("geometricType", geoType)) {
        zones[zoneI].geometricType() = geoType;
      }
      if (startFaceI != faceI) {
        FATAL_ERROR_IN(functionName)
          << "surfZones are not ordered. Start of zone " << zoneI
          << " does not correspond to sum of preceding zones." << nl
          << "while reading " << io.objectPath() << endl
          << exit(FatalError);
      }
      faceI += zoneSize;
    }
    // Check state of IOstream
    is.check(functionName.c_str());
    close();
  }
}


mousse::surfZoneIOList::surfZoneIOList
(
  const IOobject& io,
  const surfZoneList& zones
)
:
  surfZoneList{zones},
  regIOobject{io}
{}


mousse::surfZoneIOList::surfZoneIOList
(
  const IOobject& io,
  const Xfer<surfZoneList>& zones
)
:
  surfZoneList{zones},
  regIOobject{io}
{}


// Destructor
mousse::surfZoneIOList::~surfZoneIOList()
{}


// Member Functions
// writeData member function required by regIOobject
bool mousse::surfZoneIOList::writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}


// Friend Operators
mousse::Ostream& mousse::operator<<(Ostream& os, const surfZoneIOList& L)
{
  os << L.size() << nl << token::BEGIN_LIST << incrIndent << nl;
  FOR_ALL(L, i) {
    L[i].writeDict(os);
  }
  os << decrIndent << token::END_LIST;
  return os;
}

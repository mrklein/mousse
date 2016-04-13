// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_format.hpp"
#include "ifstream.hpp"
#include "iomanip.hpp"
#include "istring_stream.hpp"
#include "map.hpp"


// Constructors 
mousse::fileFormats::TRIsurfaceFormatCore::TRIsurfaceFormatCore
(
  const fileName& filename
)
:
  sorted_{true},
  points_{0},
  zoneIds_{0},
  sizes_{0}
{
  read(filename);
}


// Destructor 
mousse::fileFormats::TRIsurfaceFormatCore::~TRIsurfaceFormatCore()
{}


// Member Functions 
bool mousse::fileFormats::TRIsurfaceFormatCore::read
(
  const fileName& filename
)
{
  this->clear();
  sorted_ = true;
  IFstream is{filename};
  if (!is.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::TRIsurfaceFormatCore::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  // uses similar structure as STL, just some points
  // the rest of the reader resembles the STL binary reader
  DynamicList<point> dynPoints;
  DynamicList<label> dynZones;
  DynamicList<label> dynSizes;
  HashTable<label>   lookup;
  // place faces without a group in zone0
  label zoneI = 0;
  dynSizes.append(zoneI);
  lookup.insert("zoneI", zoneI);
  while (is.good()) {
    string line = this->getLineNoComment(is);
    IStringStream lineStream{line};
    point p
    {
      readScalar(lineStream),
      readScalar(lineStream),
      readScalar(lineStream)
    };
    if (!lineStream)
      break;
    dynPoints.append(p);
    dynPoints.append
    (
      point
      {
        readScalar(lineStream),
        readScalar(lineStream),
        readScalar(lineStream)
      }
    );
    dynPoints.append
    (
      point
      {
        readScalar(lineStream),
        readScalar(lineStream),
        readScalar(lineStream)
      }
    );
    // zone/colour in .tri file starts with 0x. Skip.
    // ie, instead of having 0xFF, skip 0 and leave xFF to
    // get read as a word and name it "zoneFF"
    char zero;
    lineStream >> zero;
    word rawName{lineStream};
    word name{"zone" + rawName(1, rawName.size()-1)};
    HashTable<label>::const_iterator fnd = lookup.find(name);
    if (fnd != lookup.end()) {
      if (zoneI != fnd()) {
        // group appeared out of order
        sorted_ = false;
      }
      zoneI = fnd();
    } else {
      zoneI = dynSizes.size();
      lookup.insert(name, zoneI);
      dynSizes.append(0);
    }
    dynZones.append(zoneI);
    dynSizes[zoneI]++;
  }
  // skip empty groups
  label nZone = 0;
  FOR_ALL(dynSizes, zoneI) {
    if (dynSizes[zoneI]) {
      if (nZone != zoneI) {
        dynSizes[nZone] = dynSizes[zoneI];
      }
      nZone++;
    }
  }
  // truncate addressed size
  dynSizes.setCapacity(nZone);
  // transfer to normal lists
  points_.transfer(dynPoints);
  zoneIds_.transfer(dynZones);
  sizes_.transfer(dynSizes);
  return true;
}
// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "starcd_core.hpp"
#include "list_ops.hpp"
#include "clock.hpp"
#include "packed_bool_list.hpp"
#include "istring_stream.hpp"
// Constructors 
mousse::fileFormats::STARCDCore::STARCDCore()
{}
//  Protected Member Functions
bool mousse::fileFormats::STARCDCore::readHeader
(
  IFstream& is,
  const word& signature
)
{
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::STARCDCore::readHeader(...)"
    )
      << "cannot read " << signature  << "  " << is.name()
      << abort(FatalError);
  }
  word header;
  label majorVersion;
  string line;
  is.getLine(line);
  IStringStream(line)() >> header;
  is.getLine(line);
  IStringStream(line)() >> majorVersion;
  // add other checks ...
  if (header != signature)
  {
    Info<< "header mismatch " << signature << "  " << is.name()
      << endl;
  }
  return true;
}
void mousse::fileFormats::STARCDCore::writeHeader
(
  Ostream& os,
  const word& filetype
)
{
  os  << "PROSTAR_" << filetype << nl
    << 4000
    << " " << 0
    << " " << 0
    << " " << 0
    << " " << 0
    << " " << 0
    << " " << 0
    << " " << 0
    << endl;
}
// Member Functions 
bool mousse::fileFormats::STARCDCore::readPoints
(
  IFstream& is,
  pointField& points,
  labelList& ids
)
{
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::STARCDedgeFormat::readPoints(...)"
    )
      << "Cannot read file " << is.name()
      << exit(FatalError);
  }
  readHeader(is, "PROSTAR_VERTEX");
  // reuse memory if possible
  DynamicList<point> dynPoints(points.xfer());
  DynamicList<label> dynPointId(ids.xfer());    // STAR-CD index of points
  dynPoints.clear();
  dynPointId.clear();
  label lineLabel;
  while ((is >> lineLabel).good())
  {
    scalar x, y, z;
    is >> x >> y >> z;
    dynPoints.append(point(x, y, z));
    dynPointId.append(lineLabel);
  }
  points.transfer(dynPoints);
  ids.transfer(dynPointId);
  return true;
}
void mousse::fileFormats::STARCDCore::writePoints
(
  Ostream& os,
  const pointField& pointLst
)
{
  writeHeader(os, "VERTEX");
  // Set the precision of the points data to 10
  os.precision(10);
  // force decimal point for Fortran input
  os.setf(std::ios::showpoint);
  forAll(pointLst, ptI)
  {
    os
      << ptI + 1 << " "
      << pointLst[ptI].x() << " "
      << pointLst[ptI].y() << " "
      << pointLst[ptI].z() << nl;
  }
  os.flush();
}

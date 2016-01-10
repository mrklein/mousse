// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "osstream.hpp"
// Constructors 
inline mousse::OSstream::OSstream
(
  ostream& os,
  const string& name,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  Ostream(format, version, compression),
  name_(name),
  os_(os)
{
  if (os_.good())
  {
    setOpened();
    setGood();
    os_.precision(precision_);
  }
  else
  {
    setState(os_.rdstate());
  }
}

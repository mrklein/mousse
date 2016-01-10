// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ofstream.hpp"
#include "os_specific.hpp"
#include "gzstream.h"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(OFstream, 0);
}
mousse::OFstreamAllocator::OFstreamAllocator
(
  const fileName& pathname,
  IOstream::compressionType compression
)
:
  ofPtr_(NULL)
{
  if (pathname.empty())
  {
    if (OFstream::debug)
    {
      Info<< "OFstreamAllocator::OFstreamAllocator(const fileName&) : "
         "cannot open null file " << endl;
    }
  }
  if (compression == IOstream::COMPRESSED)
  {
    // get identically named uncompressed version out of the way
    if (isFile(pathname, false))
    {
      rm(pathname);
    }
    ofPtr_ = new ogzstream((pathname + ".gz").c_str());
  }
  else
  {
    // get identically named compressed version out of the way
    if (isFile(pathname + ".gz", false))
    {
      rm(pathname + ".gz");
    }
    ofPtr_ = new ofstream(pathname.c_str());
  }
}
mousse::OFstreamAllocator::~OFstreamAllocator()
{
  delete ofPtr_;
}
// Constructors 
mousse::OFstream::OFstream
(
  const fileName& pathname,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  OFstreamAllocator(pathname, compression),
  OSstream(*ofPtr_, "OFstream.sinkFile_", format, version, compression),
  pathname_(pathname)
{
  setClosed();
  setState(ofPtr_->rdstate());
  if (!good())
  {
    if (debug)
    {
      Info<< "OFstream::OFstream(const fileName&,"
         "streamFormat, versionNumber, compressionType) : "
         "could not open file " << pathname
        << "for input\n"
         "in stream " << info() << mousse::endl;
    }
    setBad();
  }
  else
  {
    setOpened();
  }
  lineNumber_ = 1;
}
// Destructor 
mousse::OFstream::~OFstream()
{}
// Member Functions 
std::ostream& mousse::OFstream::stdStream()
{
  if (!ofPtr_)
  {
    FATAL_ERROR_IN("OFstream::stdStream()")
      << "No stream allocated." << abort(FatalError);
  }
  return *ofPtr_;
}
const std::ostream& mousse::OFstream::stdStream() const
{
  if (!ofPtr_)
  {
    FATAL_ERROR_IN("OFstreamAllocator::stdStream() const")
      << "No stream allocated." << abort(FatalError);
  }
  return *ofPtr_;
}
void mousse::OFstream::print(Ostream& os) const
{
  os  << "    OFstream: ";
  OSstream::print(os);
}

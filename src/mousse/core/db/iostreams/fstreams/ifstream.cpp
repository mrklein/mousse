// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ifstream.hpp"
#include "os_specific.hpp"
#include "gzstream.h"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(IFstream, 0);

}


mousse::IFstreamAllocator::IFstreamAllocator(const fileName& pathname)
:
  ifPtr_{nullptr},
  compression_{IOstream::UNCOMPRESSED}
{
  if (pathname.empty()) {
    if (IFstream::debug) {
      Info << "IFstreamAllocator::IFstreamAllocator(const fileName&) : "
          "cannot open null file " << endl;
    }
  }
  ifPtr_ = new ifstream{pathname.c_str()};
  // If the file is compressed, decompress it before reading.
  if (!ifPtr_->good() && isFile(pathname + ".gz", false)) {
    if (IFstream::debug) {
      Info << "IFstreamAllocator::IFstreamAllocator(const fileName&) : "
          "decompressing " << pathname + ".gz" << endl;
    }
    delete ifPtr_;
    ifPtr_ = new igzstream{(pathname + ".gz").c_str()};
    if (ifPtr_->good()) {
      compression_ = IOstream::COMPRESSED;
    }
  }
}


mousse::IFstreamAllocator::~IFstreamAllocator()
{
  delete ifPtr_;
}


// Constructors 
mousse::IFstream::IFstream
(
  const fileName& pathname,
  streamFormat format,
  versionNumber version
)
:
  IFstreamAllocator{pathname},
  ISstream
  {
    *ifPtr_,
    "IFstream.sourceFile_",
    format,
    version,
    IFstreamAllocator::compression_
  },
  pathname_{pathname}
{
  setClosed();
  setState(ifPtr_->rdstate());
  if (!good()) {
    if (debug) {
      Info << "IFstream::IFstream(const fileName&,"
         "streamFormat=ASCII,"
         "versionNumber=currentVersion) : "
         "could not open file for input"
        << endl << info() << endl;
    }
    setBad();
  } else {
    setOpened();
  }
  lineNumber_ = 1;
}


// Constructors 
mousse::IFstream::~IFstream()
{}


// Member Functions 
std::istream& mousse::IFstream::stdStream()
{
  if (!ifPtr_) {
    FATAL_ERROR_IN("IFstream::stdStream()")
      << "No stream allocated" << abort(FatalError);
  }
  return *ifPtr_;
}


const std::istream& mousse::IFstream::stdStream() const
{
  if (!ifPtr_) {
    FATAL_ERROR_IN("IFstream::stdStream() const")
      << "No stream allocated" << abort(FatalError);
  }
  return *ifPtr_;
}


void mousse::IFstream::print(Ostream& os) const
{
  // Print File data
  os << "IFstream: ";
  ISstream::print(os);
}


// Member Operators 
mousse::IFstream& mousse::IFstream::operator()() const
{
  if (!good()) {
    // also checks .gz file
    if (isFile(pathname_, true)) {
      check("IFstream::operator()");
      FatalIOError.exit();
    } else {
      FATAL_IO_ERROR_IN("IFstream::operator()", *this)
        << "file " << pathname_ << " does not exist"
        << exit(FatalIOError);
    }
  }
  return const_cast<IFstream&>(*this);
}


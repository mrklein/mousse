// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reg_ioobject.hpp"
#include "time.hpp"
#include "os_specific.hpp"
#include "ofstream.hpp"


bool mousse::regIOobject::writeObject
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  if (!good()) {
    SERIOUS_ERROR_IN("regIOobject::write()")
      << "bad object " << name()
      << endl;
    return false;
  }
  if (instance().empty()) {
    SERIOUS_ERROR_IN("regIOobject::write()")
      << "instance undefined for object " << name()
      << endl;
    return false;
  }
  if (instance() != time().timeName()
      && instance() != time().system()
      && instance() != time().caseSystem()
      && instance() != time().constant()
      && instance() != time().caseConstant()) {
    const_cast<regIOobject&>(*this).instance() = time().timeName();
  }
  mkDir(path());
  if (OFstream::debug) {
    Info << "regIOobject::write() : "
      << "writing file " << objectPath();
  }

  bool osGood = false;

  {
    // Try opening an OFstream for object
    OFstream os{objectPath(), fmt, ver, cmp};
    // If any of these fail, return (leave error handling to Ostream class)
    if (!os.good()) {
      return false;
    }
    if (!writeHeader(os)) {
      return false;
    }
    // Write the data to the Ostream
    if (!writeData(os)) {
      return false;
    }
    writeEndDivider(os);
    osGood = os.good();
  }
  if (OFstream::debug) {
    Info << " .... written" << endl;
  }
  // Only update the lastModified_ time if this object is re-readable,
  // i.e. lastModified_ is already set
  if (watchIndex_ != -1) {
    time().setUnmodified(watchIndex_);
  }
  return osGood;
}


bool mousse::regIOobject::write() const
{
  return writeObject
  (
    time().writeFormat(),
    IOstream::currentVersion,
    time().writeCompression()
  );
}

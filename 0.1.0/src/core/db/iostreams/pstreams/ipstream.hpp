// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IPstream
// Description
//   Input inter-processor communications stream.
// SourceFiles
//   ipstream.cpp
#include "pstream.hpp"
#ifndef ipstream_hpp_
#define ipstream_hpp_
#include "uipstream.hpp"
namespace mousse
{
class IPstream
:
  public Pstream,
  public UIPstream
{
  //- Receive index
  label externalBufPosition_;
public:
  // Constructors
    //- Construct given process index to read from and optional buffer size,
    //  read format and IO version
    IPstream
    (
      const commsTypes commsType,
      const int fromProcNo,
      const label bufSize = 0,
      const int tag = UPstream::msgType(),
      const label comm = UPstream::worldComm,
      streamFormat format=BINARY,
      versionNumber version=currentVersion
    );
};
}  // namespace mousse
#endif

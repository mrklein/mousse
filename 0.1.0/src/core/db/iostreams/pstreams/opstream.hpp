// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OPstream
// Description
//   Output inter-processor communications stream.
// SourceFiles
//   opstream.cpp
#include "pstream.hpp"
#ifndef opstream_hpp_
#define opstream_hpp_
#include "uopstream.hpp"
namespace mousse
{
class OPstream
:
  public Pstream,
  public UOPstream
{
public:
  // Constructors
    //- Construct given process index to send to and optional buffer size,
    //  write format and IO version
    OPstream
    (
      const commsTypes commsType,
      const int toProcNo,
      const label bufSize = 0,
      const int tag = UPstream::msgType(),
      const label comm = UPstream::worldComm,
      streamFormat format=BINARY,
      versionNumber version=currentVersion
    );
};
}  // namespace mousse
#endif
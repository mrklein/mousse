// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "opstream.hpp"


// Constructor
mousse::OPstream::OPstream
(
  const commsTypes commsType,
  const int toProcNo,
  const label bufSize,
  const int tag,
  const label comm,
  streamFormat format,
  versionNumber version
)
:
  Pstream{commsType, bufSize},
  UOPstream{commsType, toProcNo, buf_, tag, comm, true, format, version}
{}

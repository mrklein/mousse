// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uipstream.hpp"


// Constructor

mousse::UIPstream::UIPstream
(
  const commsTypes commsType,
  const int fromProcNo,
  DynamicList<char>& externalBuf,
  label& externalBufPosition,
  const int tag,
  const label comm,
  const bool clearAtEnd,
  streamFormat format,
  versionNumber version
)
:
  UPstream{commsType},
  Istream{format, version},
  fromProcNo_{fromProcNo},
  externalBuf_{externalBuf},
  externalBufPosition_{externalBufPosition},
  tag_{tag},
  comm_{comm},
  clearAtEnd_{clearAtEnd},
  messageSize_{0}
{
  NOT_IMPLEMENTED
  (
    "UIPstream::UIPstream\n"
    "(\n"
      "const commsTypes,\n"
      "const int,\n"
      "DynamicList<char>&,\n"
      "label&,\n"
      "const int,\n"
      "const label,\n"
      "const bool,\n"
      "streamFormat,\n"
      "versionNumber\n"
    ")"
  );
}


mousse::UIPstream::UIPstream(const int fromProcNo, PstreamBuffers& buffers)
:
  UPstream{buffers.commsType_},
  Istream{buffers.format_, buffers.version_},
  fromProcNo_{fromProcNo},
  externalBuf_{buffers.recvBuf_[fromProcNo]},
  externalBufPosition_{buffers.recvBufPos_[fromProcNo]},
  tag_{buffers.tag_},
  comm_{buffers.comm_},
  clearAtEnd_{true},
  messageSize_{0}
{
  NOT_IMPLEMENTED
  (
    "UIPstream::UIPstream\n"
    "(\n"
      "const int,\n"
      "PstreamBuffers&\n"
    ")"
  );
}


// Member Functions 
mousse::label mousse::UIPstream::read
(
  const commsTypes,
  const int /*fromProcNo*/,
  char* /*buf*/,
  const std::streamsize /*bufSize*/,
  const int /*tag*/,
  const label /*communicator*/
)
{
  NOT_IMPLEMENTED
  (
    "UIPstream::read"
    "("
      "const commsTypes,"
      "const int fromProcNo,"
      "char* buf,"
      "const label bufSize,"
      "const int tag,"
      "const label communicator"
    ")"
  );
  return 0;
}


#include "pstream.hpp"

#ifndef CORE_DB_IOSTREAMS_PSTREAMS_PSTREAM_BUFFERS_HPP_
#define CORE_DB_IOSTREAMS_PSTREAMS_PSTREAM_BUFFERS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PstreamBuffers
// Description
//   Buffers for inter-processor communications streams (UOPstream, UIPstream).
//   Use UOPstream to stream data into buffers, call finishedSends() to
//   notify that data is in buffers and then use IUPstream to get data out
//   of received buffers. Works with both blocking and nonBlocking. Does
//   not make much sense with scheduled since there you would not need these
//   explicit buffers.
//   Example usage:
//     PstreamBuffers pBuffers(Pstream::nonBlocking);
//     for (label procI = 0; procI < Pstream::nProcs(); procI++)
//     {
//       if (procI != Pstream::myProcNo())
//       {
//         someObject vals;
//         UOPstream str(procI, pBuffers);
//         str << vals;
//       }
//     }
//     pBuffers.finishedSends();   // no-op for blocking
//     for (label procI = 0; procI < Pstream::nProcs(); procI++)
//     {
//       if (procI != Pstream::myProcNo())
//       {
//         UIPstream str(procI, pBuffers);
//         someObject vals(str);
//       }
//     }

#include "dynamic_list.hpp"
#include "upstream.hpp"
#include "iostream.hpp"


namespace mousse {

class PstreamBuffers
{
  friend class UOPstream;
  friend class UIPstream;
  // Private data
    //- Communications type of this stream
    const UPstream::commsTypes commsType_;
    const int tag_;
    const label comm_;
    const IOstream::streamFormat format_;
    const IOstream::versionNumber version_;
    //- Send buffer
    List<DynamicList<char> > sendBuf_;
    //- Receive buffer
    List<DynamicList<char> > recvBuf_;
    //- Read position in recvBuf_
    labelList recvBufPos_;
    bool finishedSendsCalled_;
public:
  // Static data
    static DynamicList<char> nullBuf;
  // Constructors
    //- Construct given comms type,
    //  write format and IO version
    PstreamBuffers
    (
      const UPstream::commsTypes commsType,
      const int tag = UPstream::msgType(),
      const label comm = UPstream::worldComm,
      IOstream::streamFormat format=IOstream::BINARY,
      IOstream::versionNumber version=IOstream::currentVersion
    );
  //- Destructor
  ~PstreamBuffers();
  // Member functions
    int tag() const
    {
      return tag_;
    }
    //- Mark all sends as having been done. This will start receives
    //  in non-blocking mode. If block will wait for all transfers to
    //  finish (only relevant for nonBlocking mode)
    void finishedSends(const bool block = true);
    //- Mark all sends as having been done. Same as above but also returns
    //  sizes (bytes) transferred. Note:currently only valid for
    //  non-blocking.
    void finishedSends(labelListList& sizes, const bool block = true);
    //- Clear storage and reset
    void clear();
};
}  // namespace mousse
#endif
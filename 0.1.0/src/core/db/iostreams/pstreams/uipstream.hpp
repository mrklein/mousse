#include "pstream.hpp"

#ifndef CORE_DB_IOSTREAMS_PSTREAMS_UIPSTREAM_HPP_
#define CORE_DB_IOSTREAMS_PSTREAMS_UIPSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UIPstream
// Description
//   Input inter-processor communications stream operating on external
//   buffer.

#include "upstream.hpp"
#include "istream.hpp"
#include "pstream_buffers.hpp"


namespace mousse {

class UIPstream
:
  public UPstream,
  public Istream
{
  // Private data
    int fromProcNo_;
    DynamicList<char>& externalBuf_;
    label& externalBufPosition_;
    const int tag_;
    const label comm_;
    const bool clearAtEnd_;
    int messageSize_;
  // Private Member Functions
    //- Check the bufferPosition against messageSize_ for EOF
    inline void checkEof();
    //- Read a T from the transfer buffer
    template<class T>
    inline void readFromBuffer(T&);
    //- Read data from the transfer buffer
    inline void readFromBuffer(void* data, size_t count, size_t align);
public:
  // Constructors
    //- Construct given process index to read from and optional buffer size,
    //  read format and IO version
    UIPstream
    (
      const commsTypes commsType,
      const int fromProcNo,
      DynamicList<char>& externalBuf,
      label& externalBufPosition,
      const int tag = UPstream::msgType(),
      const label comm = UPstream::worldComm,
      const bool clearAtEnd = false,   // destroy externalBuf if at end
      streamFormat format=BINARY,
      versionNumber version=currentVersion
    );
    //- Construct given buffers
    UIPstream(const int fromProcNo, PstreamBuffers&);
  //- Destructor
  ~UIPstream();
  // Member functions
    // Inquiry
      //- Return flags of output stream
      ios_base::fmtflags flags() const
      {
        return ios_base::fmtflags(0);
      }
    // Read functions
      //- Read into given buffer from given processor and return the
      //  message size
      static label read
      (
        const commsTypes commsType,
        const int fromProcNo,
        char* buf,
        const std::streamsize bufSize,
        const int tag = UPstream::msgType(),
        const label communicator = 0
      );
      //- Return next token from stream
      Istream& read(token&);
      //- Read a character
      Istream& read(char&);
      //- Read a word
      Istream& read(word&);
      // Read a string (including enclosing double-quotes)
      Istream& read(string&);
      //- Read a label
      Istream& read(label&);
      //- Read a floatScalar
      Istream& read(floatScalar&);
      //- Read a doubleScalar
      Istream& read(doubleScalar&);
      //- Read binary block
      Istream& read(char*, std::streamsize);
      //- Rewind and return the stream so that it may be read again
      Istream& rewind();
    // Edit
      //- Set flags of stream
      ios_base::fmtflags flags(const ios_base::fmtflags)
      {
        return ios_base::fmtflags(0);
      }
    // Print
      //- Print description of IOstream to Ostream
      void print(Ostream&) const;
};
}  // namespace mousse
#endif

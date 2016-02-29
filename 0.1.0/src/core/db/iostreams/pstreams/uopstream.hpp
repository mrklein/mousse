#include "pstream.hpp"

#ifndef CORE_DB_IOSTREAMS_PSTREAMS_UOPSTREAM_HPP_
#define CORE_DB_IOSTREAMS_PSTREAMS_UOPSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UOPstream
// Description
//   Output inter-processor communications stream operating on external
//   buffer.
// SourceFiles
//   uopstream.cpp

#include "upstream.hpp"
#include "ostream.hpp"
#include "dynamic_list.hpp"
#include "pstream_buffers.hpp"
namespace mousse
{
class UOPstream
:
  public UPstream,
  public Ostream
{
  // Private data
    int toProcNo_;
    DynamicList<char>& sendBuf_;
    const int tag_;
    const label comm_;
    const bool sendAtDestruct_;
  // Private Member Functions
    //- Write a T to the transfer buffer
    template<class T>
    inline void writeToBuffer(const T&);
    //- Write a char to the transfer buffer
    inline void writeToBuffer(const char&);
    //- Write data to the transfer buffer
    inline void writeToBuffer(const void* data, size_t count, size_t align);
public:
  // Constructors
    //- Construct given process index to send to and optional buffer size,
    //  write format and IO version
    UOPstream
    (
      const commsTypes commsType,
      const int toProcNo,
      DynamicList<char>& sendBuf,
      const int tag = UPstream::msgType(),
      const label comm = UPstream::worldComm,
      const bool sendAtDestruct = true,
      streamFormat format=BINARY,
      versionNumber version=currentVersion
    );
    //- Construct given buffers
    UOPstream(const int toProcNo, PstreamBuffers&);
  //- Destructor
  ~UOPstream();
  // Member functions
    // Inquiry
      //- Return flags of output stream
      ios_base::fmtflags flags() const
      {
        return ios_base::fmtflags(0);
      }
    // Write functions
      //- Write given buffer to given processor
      static bool write
      (
        const commsTypes commsType,
        const int toProcNo,
        const char* buf,
        const std::streamsize bufSize,
        const int tag = UPstream::msgType(),
        const label communicator = 0
      );
      //- Write next token to stream
      Ostream& write(const token&);
      //- Write character
      Ostream& write(const char);
      //- Write character string
      Ostream& write(const char*);
      //- Write word
      Ostream& write(const word&);
      //- Write string
      Ostream& write(const string&);
      //- Write std::string surrounded by quotes.
      //  Optional write without quotes.
      Ostream& writeQuoted
      (
        const std::string&,
        const bool quoted=true
      );
      //- Write int32_t
      virtual Ostream& write(const int32_t);
      //- Write int64_t
      Ostream& write(const int64_t);
      //- Write floatScalar
      Ostream& write(const floatScalar);
      //- Write doubleScalar
      Ostream& write(const doubleScalar);
      //- Write binary block
      Ostream& write(const char*, std::streamsize);
      //- Add indentation characters
      void indent()
      {}
    // Stream state functions
      //- Flush stream
      void flush()
      {}
      //- Add newline and flush stream
      void endl()
      {}
      //- Get width of output field
      int width() const
      {
        return 0;
      }
      //- Set width of output field (and return old width)
      int width(const int)
      {
        return 0;
      }
      //- Get precision of output field
      int precision() const
      {
        return 0;
      }
      //- Set precision of output field (and return old precision)
      int precision(const int)
      {
        return 0;
      }
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

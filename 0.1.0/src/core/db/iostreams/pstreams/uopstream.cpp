// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "uopstream.hpp"
#include "int.hpp"
#include "token.hpp"
#include <cctype>
// Private Member Functions 
template<class T>
inline void mousse::UOPstream::writeToBuffer(const T& t)
{
  writeToBuffer(&t, sizeof(T), sizeof(T));
}
inline void mousse::UOPstream::writeToBuffer(const char& c)
{
  if (!sendBuf_.capacity())
  {
    sendBuf_.setCapacity(1000);
  }
  sendBuf_.append(c);
}
inline void mousse::UOPstream::writeToBuffer
(
  const void* data,
  size_t count,
  size_t align
)
{
  if (!sendBuf_.capacity())
  {
    sendBuf_.setCapacity(1000);
  }
  label alignedPos = sendBuf_.size();
  if (align > 1)
  {
    // Align bufPosition. Pads sendBuf_.size() - oldPos characters.
    alignedPos = align + ((sendBuf_.size() - 1) & ~(align - 1));
  }
  // Extend if necessary
  sendBuf_.setSize(alignedPos + count);
  const char* dataPtr = reinterpret_cast<const char*>(data);
  size_t i = count;
  while (i--) sendBuf_[alignedPos++] = *dataPtr++;
}
// Constructor
mousse::UOPstream::UOPstream
(
  const commsTypes commsType,
  const int toProcNo,
  DynamicList<char>& sendBuf,
  const int tag,
  const label comm,
  const bool sendAtDestruct,
  streamFormat format,
  versionNumber version
)
:
  UPstream(commsType),
  Ostream(format, version),
  toProcNo_(toProcNo),
  sendBuf_(sendBuf),
  tag_(tag),
  comm_(comm),
  sendAtDestruct_(sendAtDestruct)
{
  setOpened();
  setGood();
}
mousse::UOPstream::UOPstream(const int toProcNo, PstreamBuffers& buffers)
:
  UPstream(buffers.commsType_),
  Ostream(buffers.format_, buffers.version_),
  toProcNo_(toProcNo),
  sendBuf_(buffers.sendBuf_[toProcNo]),
  tag_(buffers.tag_),
  comm_(buffers.comm_),
  sendAtDestruct_(buffers.commsType_ != UPstream::nonBlocking)
{
  setOpened();
  setGood();
}
// Destructor 
mousse::UOPstream::~UOPstream()
{
  if (sendAtDestruct_)
  {
    if
    (
     !UOPstream::write
      (
        commsType_,
        toProcNo_,
        sendBuf_.begin(),
        sendBuf_.size(),
        tag_,
        comm_
      )
    )
    {
      FATAL_ERROR_IN("UOPstream::~UOPstream()")
        << "Failed sending outgoing message of size " << sendBuf_.size()
        << " to processor " << toProcNo_
        << mousse::abort(FatalError);
    }
  }
}
// Member Functions 
mousse::Ostream& mousse::UOPstream::write(const token& t)
{
  // Raw token output only supported for verbatim strings for now
  if (t.type() == token::VERBATIMSTRING)
  {
    write(char(token::VERBATIMSTRING));
    write(t.stringToken());
  }
  else if (t.type() == token::VARIABLE)
  {
    write(char(token::VARIABLE));
    write(t.stringToken());
  }
  else
  {
    NOT_IMPLEMENTED("Ostream& UOPstream::write(const token&)");
    setBad();
  }
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const char c)
{
  if (!isspace(c))
  {
    writeToBuffer(c);
  }
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const char* str)
{
  word nonWhiteChars(string::validate<word>(str));
  if (nonWhiteChars.size() == 1)
  {
    return write(nonWhiteChars.c_str()[1]);
  }
  else if (nonWhiteChars.size())
  {
    return write(nonWhiteChars);
  }
  else
  {
    return *this;
  }
}
mousse::Ostream& mousse::UOPstream::write(const word& str)
{
  write(char(token::WORD));
  size_t len = str.size();
  writeToBuffer(len);
  writeToBuffer(str.c_str(), len + 1, 1);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const string& str)
{
  write(char(token::STRING));
  size_t len = str.size();
  writeToBuffer(len);
  writeToBuffer(str.c_str(), len + 1, 1);
  return *this;
}
mousse::Ostream& mousse::UOPstream::writeQuoted
(
  const std::string& str,
  const bool quoted
)
{
  if (quoted)
  {
    write(char(token::STRING));
  }
  else
  {
    write(char(token::WORD));
  }
  size_t len = str.size();
  writeToBuffer(len);
  writeToBuffer(str.c_str(), len + 1, 1);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const int32_t val)
{
  write(char(token::LABEL));
  writeToBuffer(val);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const int64_t val)
{
  write(char(token::LABEL));
  writeToBuffer(val);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const floatScalar val)
{
  write(char(token::FLOAT_SCALAR));
  writeToBuffer(val);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const doubleScalar val)
{
  write(char(token::DOUBLE_SCALAR));
  writeToBuffer(val);
  return *this;
}
mousse::Ostream& mousse::UOPstream::write(const char* data, std::streamsize count)
{
  if (format() != BINARY)
  {
    FATAL_ERROR_IN("Ostream::write(const char*, std::streamsize)")
      << "stream format not binary"
      << mousse::abort(FatalError);
  }
  writeToBuffer(data, count, 8);
  return *this;
}
void mousse::UOPstream::print(Ostream& os) const
{
  os  << "Writing from processor " << toProcNo_
    << " to processor " << myProcNo() << " in communicator " << comm_
    << " and tag " << tag_ << mousse::endl;
}

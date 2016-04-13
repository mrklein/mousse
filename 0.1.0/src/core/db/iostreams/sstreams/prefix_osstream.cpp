// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "prefix_osstream.hpp"
#include "pstream.hpp"
#include "token.hpp"


// Private Member Functions 
inline void mousse::prefixOSstream::checkWritePrefix()
{
  if (printPrefix_ && prefix_.size()) {
    OSstream::write(prefix_.c_str());
    printPrefix_ = false;
  }
}
// Constructors 
mousse::prefixOSstream::prefixOSstream
(
  ostream& os,
  const string& name,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  OSstream{os, name, format, version, compression},
  printPrefix_{true},
  prefix_{""}
{}


// Member Functions 
void mousse::prefixOSstream::print(Ostream& os) const
{
  os << "prefixOSstream ";
  OSstream::print(os);
}


mousse::Ostream& mousse::prefixOSstream::write(const token& t)
{
  if (t.type() == token::VERBATIMSTRING) {
    write(char(token::HASH));
    write(char(token::BEGIN_BLOCK));
    writeQuoted(t.stringToken(), false);
    write(char(token::HASH));
    write(char(token::END_BLOCK));
  } else if (t.type() == token::VARIABLE) {
    writeQuoted(t.stringToken(), false);
  }
  return *this;
}


mousse::Ostream& mousse::prefixOSstream::write(const char c)
{
  checkWritePrefix();
  OSstream::write(c);
  if (c == token::NL) {
    printPrefix_ = true;
  }
  return *this;
}


mousse::Ostream& mousse::prefixOSstream::write(const char* str)
{
  checkWritePrefix();
  OSstream::write(str);
  size_t len = strlen(str);
  if (len && str[len-1] == token::NL) {
    printPrefix_ = true;
  }
  return *this;
}


mousse::Ostream& mousse::prefixOSstream::write(const word& val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::write(const string& val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::writeQuoted
(
  const std::string& val,
  const bool quoted
)
{
  checkWritePrefix();
  return OSstream::writeQuoted(val, quoted);
}


mousse::Ostream& mousse::prefixOSstream::write(const int32_t val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::write(const int64_t val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::write(const floatScalar val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::write(const doubleScalar val)
{
  checkWritePrefix();
  return OSstream::write(val);
}


mousse::Ostream& mousse::prefixOSstream::write
(
  const char* buf,
  std::streamsize count
)
{
  checkWritePrefix();
  return OSstream::write(buf, count);
}


void mousse::prefixOSstream::indent()
{
  checkWritePrefix();
  OSstream::indent();
}


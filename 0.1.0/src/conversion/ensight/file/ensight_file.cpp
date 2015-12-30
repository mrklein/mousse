// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_file.hpp"
#include <sstream>
// Static Data Members
bool mousse::ensightFile::allowUndef_ = false;
mousse::scalar mousse::ensightFile::undefValue_ = mousse::floatScalarVGREAT;
// default is width 8
mousse::string mousse::ensightFile::mask_ = "********";
mousse::string mousse::ensightFile::dirFmt_ = "%08d";
// Static Member Functions
mousse::string mousse::ensightFile::mask()
{
  return mask_;
}
mousse::string mousse::ensightFile::subDir(const label n)
{
  char buf[32];
  sprintf(buf, dirFmt_.c_str(), n);
  return buf;
}
void mousse::ensightFile::subDirWidth(const label n)
{
  // enforce max limit to avoid buffer overflow in subDir()
  if (n < 1 || n > 31)
  {
    return;
  }
  // appropriate printf format
  std::ostringstream oss;
  oss << "%0" << n << "d";
  dirFmt_ = oss.str();
  // set mask accordingly
  mask_.resize(n, '*');
}
mousse::label mousse::ensightFile::subDirWidth()
{
  return mask_.size();
}
// Constructors 
mousse::ensightFile::ensightFile
(
  const fileName& pathname,
  IOstream::streamFormat format
)
:
  OFstream(pathname, format)
{
  // ascii formatting specs
  setf
  (
    ios_base::scientific,
    ios_base::floatfield
  );
  precision(5);
}
// Destructor 
mousse::ensightFile::~ensightFile()
{}
// Member Functions 
bool mousse::ensightFile::allowUndef()
{
  return allowUndef_;
}
bool mousse::ensightFile::allowUndef(bool value)
{
  bool old = allowUndef_;
  allowUndef_ = value;
  return old;
}
mousse::scalar mousse::ensightFile::undefValue(const scalar value)
{
  // enable its use too
  allowUndef_ = true;
  scalar old = undefValue_;
  undefValue_ = value;
  return old;
}
mousse::Ostream& mousse::ensightFile::write
(
  const char* buf,
  std::streamsize count
)
{
  stdStream().write(buf, count);
  return *this;
}
mousse::Ostream& mousse::ensightFile::write(const char* value)
{
  return write(string(value));
}
mousse::Ostream& mousse::ensightFile::write(const string& value)
{
  char buf[80];
  for (string::size_type i = 0; i < 80; ++i)
  {
    buf[i] = 0;
  }
  string::size_type n = value.size();
  if (n >= 80)
  {
    n = 79;
  }
  for (string::size_type i = 0; i < n; ++i)
  {
    buf[i] = value[i];
  }
  if (format() == IOstream::BINARY)
  {
    write
    (
      reinterpret_cast<char const *>(buf),
      sizeof(buf)
    );
  }
  else
  {
    stdStream() << buf;
  }
  return *this;
}
mousse::Ostream& mousse::ensightFile::write(const label value)
{
  if (format() == IOstream::BINARY)
  {
    unsigned int ivalue(value);
    write
    (
      reinterpret_cast<char const *>(&ivalue),
      sizeof(ivalue)
    );
  }
  else
  {
    stdStream().width(10);
    stdStream() << value;
  }
  return *this;
}
mousse::Ostream& mousse::ensightFile::write
(
  const label value,
  const label fieldWidth
)
{
  if (format() == IOstream::BINARY)
  {
    unsigned int ivalue(value);
    write
    (
      reinterpret_cast<char const *>(&ivalue),
      sizeof(ivalue)
    );
  }
  else
  {
    stdStream().width(fieldWidth);
    stdStream() << value;
  }
  return *this;
}
mousse::Ostream& mousse::ensightFile::write(const scalar value)
{
  float fvalue(value);
  if (format() == IOstream::BINARY)
  {
    write
    (
      reinterpret_cast<char const *>(&fvalue),
      sizeof(fvalue)
    );
  }
  else
  {
    stdStream().width(12);
    stdStream() << fvalue;
  }
  return *this;
}
void mousse::ensightFile::newline()
{
  if (format() == IOstream::ASCII)
  {
    stdStream() << nl;
  }
}
mousse::Ostream& mousse::ensightFile::writeUndef()
{
  write(undefValue_);
  return *this;
}
mousse::Ostream& mousse::ensightFile::writeKeyword(const string& key)
{
  if (allowUndef_)
  {
    write(string(key + " undef"));
    newline();
    write(undefValue_);
    newline();
  }
  else
  {
    write(key);
    newline();
  }
  return *this;
}
mousse::Ostream& mousse::ensightFile::writeBinaryHeader()
{
  if (format() == IOstream::BINARY)
  {
    write("C Binary");
  }
  return *this;
}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "osstream.hpp"
#include "token.hpp"


mousse::Ostream& mousse::OSstream::write(const token& t)
{
  if (t.type() == token::VERBATIMSTRING)
  {
    write(char(token::HASH));
    write(char(token::BEGIN_BLOCK));
    writeQuoted(t.stringToken(), false);
    write(char(token::HASH));
    write(char(token::END_BLOCK));
  }
  else if (t.type() == token::VARIABLE)
  {
    writeQuoted( t.stringToken(), false);
  }
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const char c)
{
  os_ << c;
  if (c == token::NL)
  {
    lineNumber_++;
  }
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const char* str)
{
  lineNumber_ += string(str).count(token::NL);
  os_ << str;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const word& str)
{
  os_ << str;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const string& str)
{
  os_ << token::BEGIN_STRING;
  int backslash = 0;
  for (string::const_iterator iter = str.begin(); iter != str.end(); ++iter)
  {
    char c = *iter;
    if (c == '\\')
    {
      backslash++;
      // suppress output until we know if other characters follow
      continue;
    }
    else if (c == token::NL)
    {
      lineNumber_++;
      backslash++;    // backslash escape for newline
    }
    else if (c == token::END_STRING)
    {
      backslash++;    // backslash escape for quote
    }
    // output pending backslashes
    while (backslash)
    {
      os_ << '\\';
      backslash--;
    }
    os_ << c;
  }
  // silently drop any trailing backslashes
  // they would otherwise appear like an escaped end-quote
  os_ << token::END_STRING;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::writeQuoted
(
  const std::string& str,
  const bool quoted
)
{
  if (quoted)
  {
    os_ << token::BEGIN_STRING;
    int backslash = 0;
    for
    (
      string::const_iterator iter = str.begin();
      iter != str.end();
      ++iter
    )
    {
      char c = *iter;
      if (c == '\\')
      {
        backslash++;
        // suppress output until we know if other characters follow
        continue;
      }
      else if (c == token::NL)
      {
        lineNumber_++;
        backslash++;    // backslash escape for newline
      }
      else if (c == token::END_STRING)
      {
        backslash++;    // backslash escape for quote
      }
      // output pending backslashes
      while (backslash)
      {
        os_ << '\\';
        backslash--;
      }
      os_ << c;
    }
    // silently drop any trailing backslashes
    // they would otherwise appear like an escaped end-quote
    os_ << token::END_STRING;
  }
  else
  {
    // output unquoted string, only advance line number on newline
    lineNumber_ += string(str).count(token::NL);
    os_ << str;
  }
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const int32_t val)
{
  os_ << val;
  setState(os_.rdstate());
  return *this;
}
mousse::Ostream& mousse::OSstream::write(const int64_t val)
{
  os_ << val;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const floatScalar val)
{
  os_ << val;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const doubleScalar val)
{
  os_ << val;
  setState(os_.rdstate());
  return *this;
}


mousse::Ostream& mousse::OSstream::write(const char* buf, std::streamsize count)
{
  if (format() != BINARY)
  {
    FATAL_IO_ERROR_IN("Ostream::write(const char*, std::streamsize)", *this)
      << "stream format not binary"
      << abort(FatalIOError);
  }
  os_ << token::BEGIN_LIST;
  os_.write(buf, count);
  os_ << token::END_LIST;
  setState(os_.rdstate());
  return *this;
}


void mousse::OSstream::indent()
{
  for (unsigned short i = 0; i < indentLevel_*indentSize_; i++)
  {
    os_ << ' ';
  }
}


void mousse::OSstream::flush()
{
  os_.flush();
}


// Add carriage return and flush stream
void mousse::OSstream::endl()
{
  write('\n');
  os_.flush();
}


// Get flags of output stream
std::ios_base::fmtflags mousse::OSstream::flags() const
{
  return os_.flags();
}


// Set flags of output stream
std::ios_base::fmtflags mousse::OSstream::flags(const ios_base::fmtflags f)
{
  return os_.flags(f);
}


// Get width of output field
int mousse::OSstream::width() const
{
  return os_.width();
}


// Set width of output field (and return old width)
int mousse::OSstream::width(const int w)
{
  return os_.width(w);
}


// Get precision of output field
int mousse::OSstream::precision() const
{
  return os_.precision();
}


// Set precision of output field (and return old precision)
int mousse::OSstream::precision(const int p)
{
  return os_.precision(p);
}

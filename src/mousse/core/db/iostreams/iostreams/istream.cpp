// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "istream.hpp"

// Member Functions 
void mousse::Istream::putBack(const token& t)
{
  if (bad())
  {
    FATAL_IO_ERROR_IN("void Istream::putBack(const token&)", *this)
      << "Attempt to put back onto bad stream"
      << exit(FatalIOError);
  }
  else if (putBack_)
  {
    FATAL_IO_ERROR_IN("void Istream::putBack(const token&)", *this)
      << "Attempt to put back another token"
      << exit(FatalIOError);
  }
  else
  {
    putBackToken_ = t;
    putBack_ = true;
  }
}
bool mousse::Istream::getBack(token& t)
{
  if (bad())
  {
    FATAL_IO_ERROR_IN("void Istream::getBack(token&)", *this)
      << "Attempt to get back from bad stream"
      << exit(FatalIOError);
  }
  else if (putBack_)
  {
    t = putBackToken_;
    putBack_ = false;
    return true;
  }
  return false;
}
bool mousse::Istream::peekBack(token& t)
{
  if (putBack_)
  {
    t = putBackToken_;
  }
  else
  {
    t = token::undefinedToken;
  }
  return putBack_;
}
// Functions for reading object delimiters ( ... )
mousse::Istream& mousse::Istream::readBegin(const char* funcName)
{
  token delimiter(*this);
  if (delimiter != token::BEGIN_LIST)
  {
    setBad();
    FATAL_IO_ERROR_IN("Istream::readBegin(const char*)", *this)
      << "Expected a '" << token::BEGIN_LIST
      << "' while reading " << funcName
      << ", found " << delimiter.info()
      << exit(FatalIOError);
  }
  return *this;
}
mousse::Istream& mousse::Istream::readEnd(const char* funcName)
{
  token delimiter(*this);
  if (delimiter != token::END_LIST)
  {
    setBad();
    FATAL_IO_ERROR_IN("Istream::readEnd(const char*)", *this)
      << "Expected a '" << token::END_LIST
      << "' while reading " << funcName
      << ", found " << delimiter.info()
      << exit(FatalIOError);
  }
  return *this;
}
mousse::Istream& mousse::Istream::readEndBegin(const char* funcName)
{
  readEnd(funcName);
  return readBegin(funcName);
}
// Functions for reading List delimiters ( ... ) or { ... }
char mousse::Istream::readBeginList(const char* funcName)
{
  token delimiter(*this);
  if (delimiter != token::BEGIN_LIST && delimiter != token::BEGIN_BLOCK)
  {
    setBad();
    FATAL_IO_ERROR_IN("Istream::readBeginList(const char*)", *this)
      << "Expected a '" << token::BEGIN_LIST
      << "' or a '" << token::BEGIN_BLOCK
      << "' while reading " << funcName
      << ", found " << delimiter.info()
      << exit(FatalIOError);
    return '\0';
  }
  return delimiter.pToken();
}
char mousse::Istream::readEndList(const char* funcName)
{
  token delimiter(*this);
  if (delimiter != token::END_LIST && delimiter != token::END_BLOCK)
  {
    setBad();
    FATAL_IO_ERROR_IN("Istream::readEndList(const char*)", *this)
      << "Expected a '" << token::END_LIST
      << "' or a '" << token::END_BLOCK
      << "' while reading " << funcName
      << ", found " << delimiter.info()
      << exit(FatalIOError);
    return '\0';
  }
  return delimiter.pToken();
}
mousse::Istream& mousse::Istream::operator()() const
{
  if (!good())
  {
    check("Istream::operator()");
    FatalIOError.exit();
  }
  return const_cast<Istream&>(*this);
}

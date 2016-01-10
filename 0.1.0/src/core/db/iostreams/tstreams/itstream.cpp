// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "itstream.hpp"

void mousse::ITstream::print(Ostream& os) const
{
  os  << "ITstream : " << name_.c_str();
  if (size())
  {
    if (begin()->lineNumber() == rbegin()->lineNumber())
    {
      os  << ", line " << begin()->lineNumber() << ", ";
    }
    else
    {
      os  << ", lines " << begin()->lineNumber()
        << '-' << rbegin()->lineNumber() << ", ";
    }
  }
  else
  {
    os  << ", line " << lineNumber() << ", ";
  }
  IOstream::print(os);
}


mousse::Istream& mousse::ITstream::read(token& t)
{
  // Return the put back token if it exists
  if (Istream::getBack(t))
  {
    lineNumber_ = t.lineNumber();
    return *this;
  }
  if (tokenIndex_ < size())
  {
    t = operator[](tokenIndex_++);
    lineNumber_ = t.lineNumber();
    if (tokenIndex_ == size())
    {
      setEof();
    }
  }
  else
  {
    if (eof())
    {
      FATAL_IO_ERROR_IN
      (
        "ITstream::read(token&)",
        *this
      )   << "attempt to read beyond EOF"
        << exit(FatalIOError);
      setBad();
    }
    else
    {
      setEof();
    }
    t = token::undefinedToken;
    if (size())
    {
      t.lineNumber() = tokenList::last().lineNumber();
    }
    else
    {
      t.lineNumber() = lineNumber();
    }
  }
  return *this;
}


mousse::Istream& mousse::ITstream::read(char&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(char&)");
  return *this;
}


mousse::Istream& mousse::ITstream::read(word&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(word&)");
  return *this;
}


mousse::Istream& mousse::ITstream::read(string&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(string&)");
  return *this;
}


mousse::Istream& mousse::ITstream::read(label&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(label&)");
  return *this;
}


mousse::Istream& mousse::ITstream::read(floatScalar&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(floatScalar&)");
  return *this;
}
mousse::Istream& mousse::ITstream::read(doubleScalar&)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(doubleScalar&)");
  return *this;
}
mousse::Istream& mousse::ITstream::read(char*, std::streamsize)
{
  NOT_IMPLEMENTED("Istream& ITstream::read(char*, std::streamsize)");
  return *this;
}
mousse::Istream& mousse::ITstream::rewind()
{
  tokenIndex_ = 0;
  if (size())
  {
    lineNumber_ = tokenList::first().lineNumber();
  }
  setGood();
  return *this;
}

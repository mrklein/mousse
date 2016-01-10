// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "isstream.hpp"
// Constructors 
inline mousse::ISstream::ISstream
(
  istream& is,
  const string& name,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  Istream(format, version, compression),
  name_(name),
  is_(is)
{
  if (is_.good())
  {
    setOpened();
    setGood();
  }
  else
  {
    setState(is_.rdstate());
  }
}
// Member Functions 
inline mousse::ISstream& mousse::ISstream::get(char& c)
{
  is_.get(c);
  setState(is_.rdstate());
  if (c == '\n')
  {
    lineNumber_++;
  }
  return *this;
}
inline int mousse::ISstream::peek()
{
  return is_.peek();
}
inline mousse::ISstream& mousse::ISstream::getLine(string& s)
{
  getline(is_, s);
  setState(is_.rdstate());
  lineNumber_++;
  return *this;
}
inline mousse::ISstream& mousse::ISstream::putback(const char& c)
{
  if (c == '\n')
  {
    lineNumber_--;
  }
  if (!is_.putback(c))
  {
    setBad();
  }
  setState(is_.rdstate());
  return *this;
}

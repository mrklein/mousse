// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "char.hpp"
#include "iostreams.hpp"


mousse::Istream& mousse::operator>>(Istream& is, char& c)
{
  is.read(c);
  is.check("Istream& operator>>(Istream&, char&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const char c)
{
  os.write(c);
  os.check("Ostream& operator<<(Ostream&, const char)");
  return os;
}


char mousse::readChar(Istream& is)
{
 char c;
 is.read(c);
 return c;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const char* s)
{
  os.write(s);
  os.check("Ostream& operator<<(Ostream&, const char*)");
  return os;
}


// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "string.hpp"
#include "iostreams.hpp"

// Constructors 
mousse::string::string(Istream& is)
{
  is >> *this;
}

// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, string& s)
{
  token t(is);
  if (!t.good())
  {
    is.setBad();
    return is;
  }
  if (t.isString())
  {
    s = t.stringToken();
  }
  else
  {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, string&)", is)
      << "wrong token type - expected string, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, string&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const string& s)
{
  os.write(s);
  os.check("Ostream& operator<<(Ostream&, const string&)");
  return os;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const std::string& s)
{
  os.write(string(s));
  os.check("Ostream& operator<<(Ostream&, const std::string&)");
  return os;
}

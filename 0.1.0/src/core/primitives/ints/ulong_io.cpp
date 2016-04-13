// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ulong.hpp"
#include "iostreams.hpp"
#include <sstream>


mousse::word mousse::name(const unsigned long val)
{
  std::ostringstream buf;
  buf << val;
  return buf.str();
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, unsigned long& i)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isLabel()) {
    i = static_cast<unsigned long>(t.labelToken());
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, unsigned long&)", is)
      << "wrong token type - expected unsigned long, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, unsigned long&)");
  return is;
}


unsigned long mousse::readUlong(Istream& is)
{
  unsigned long val;
  is >> val;
  return val;
}


bool mousse::read(const char* buf, unsigned long& s)
{
  char *endptr = NULL;
  s = strtoul(buf, &endptr, 10);
  return (*endptr == 0);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const unsigned long i)
{
  os.write(label(i));
  os.check("Ostream& operator<<(Ostream&, const unsigned long)");
  return os;
}

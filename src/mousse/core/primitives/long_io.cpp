// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "long.hpp"
#include "iostreams.hpp"
#include <inttypes.h>
#include <sstream>
#include <cerrno>


mousse::word mousse::name(const long val)
{
  std::ostringstream buf;
  buf << val;
  return buf.str();
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, long& i)
{
  token t{is};
  if (!t.good())
  {
    is.setBad();
    return is;
  }
  if (t.isLabel())
  {
    i = long(t.labelToken());
  }
  else
  {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, long&)", is)
      << "wrong token type - expected long, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, long&)");
  return is;
}


long mousse::readLong(Istream& is)
{
  long val;
  is >> val;
  return val;
}


bool mousse::read(const char* buf, long& s)
{
  char *endptr = nullptr;
  errno = 0;
  s = strtol(buf, &endptr, 10);
  return (*endptr == 0) && (errno == 0);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const long i)
{
  os.write(label(i));
  os.check("Ostream& operator<<(Ostream&, const long)");
  return os;
}

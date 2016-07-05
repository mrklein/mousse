// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "int64.hpp"
#include "iostreams.hpp"
#include <inttypes.h>
#include <sstream>
#include <cerrno>


mousse::word mousse::name(const int64_t val)
{
  std::ostringstream buf;
  buf << val;
  return buf.str();
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, int64_t& i)
{
  token t(is);
  if (!t.good())
  {
    is.setBad();
    return is;
  }
  if (t.isLabel())
  {
    i = int64_t(t.labelToken());
  }
  else
  {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, int64_t&)", is)
      << "wrong token type - expected int64_t, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, int64_t&)");
  return is;
}


int64_t mousse::readInt64(Istream& is)
{
  int64_t val;
  is >> val;
  return val;
}


bool mousse::read(const char* buf, int64_t& s)
{
  char *endptr = nullptr;
  errno = 0;
  intmax_t l = strtoimax(buf, &endptr, 10);
  s = int64_t(l);
  return (*endptr == 0) && (errno == 0);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const int64_t i)
{
  os.write(label(i));
  os.check("Ostream& operator<<(Ostream&, const int64_t)");
  return os;
}

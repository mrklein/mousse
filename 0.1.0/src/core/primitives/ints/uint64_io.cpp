// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "uint64.hpp"
#include "iostreams.hpp"
#include <sstream>


mousse::word mousse::name(const uint64_t val)
{
  std::ostringstream buf;
  buf << val;
  return buf.str();
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, uint64_t& i)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isLabel()) {
    i = uint64_t(t.labelToken());
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, uint64_t&)", is)
      << "wrong token type - expected uint64_t, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, uint64_t&)");
  return is;
}


uint64_t mousse::readUint64(Istream& is)
{
  uint64_t val;
  is >> val;
  return val;
}


bool mousse::read(const char* buf, uint64_t& s)
{
  char *endptr = NULL;
  long l = strtol(buf, &endptr, 10);
  s = uint64_t(l);
  return (*endptr == 0);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const uint64_t i)
{
  os.write(label(i));
  os.check("Ostream& operator<<(Ostream&, const uint64_t)");
  return os;
}

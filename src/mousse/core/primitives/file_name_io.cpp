// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "file_name.hpp"
#include "iostreams.hpp"


mousse::fileName::fileName(Istream& is)
:
  string()
{
  is >> *this;
}


mousse::Istream& mousse::operator>>(Istream& is, fileName& fn)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isString()) {
    fn = t.stringToken();
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, fileName&)", is)
      << "wrong token type - expected string, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  fn.stripInvalid();
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, fileName&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const fileName& fn)
{
  os.write(fn);
  os.check("Ostream& operator<<(Ostream&, const fileName&)");
  return os;
}

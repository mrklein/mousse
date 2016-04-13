// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "bool.hpp"
#include "switch.hpp"
#include "iostreams.hpp"


mousse::Istream& mousse::operator>>(Istream& is, bool& b)
{
  if (is.good())
  {
    b = Switch(is);
  }
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const bool b)
{
  // we could also write as text string without any difficulty
  // os  << (b ? "true" : "false");
  os.write(label(b));
  os.check("Ostream& operator<<(Ostream&, const bool)");
  return os;
}
bool mousse::readBool(Istream& is)
{
  bool b;
  is  >> b;
  return b;
}

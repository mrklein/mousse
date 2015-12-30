// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "classname.hpp"
#include "io_streams.hpp"
// Constructors 
mousse::CLASSNAME::CLASSNAME(Istream& is)
:
  base1(is),
  base2(is),
  member1(is),
  member2(is)
{
  // Check state of Istream
  is.check("mousse::CLASSNAME::CLASSNAME(mousse::Istream&)");
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, CLASSNAME&)
{
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>(mousse::Istream&, mousse::CLASSNAME&)"
  );
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const CLASSNAME&)
{
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::CLASSNAME&)"
  );
  return os;
}

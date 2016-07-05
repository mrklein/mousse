// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mem_info.hpp"
#include "iostreams.hpp"


// Constructors 
mousse::memInfo::memInfo(Istream& is)
:
  base1{is},
  base2{is},
  member1{is},
  member2{is}
{
  // Check state of Istream
  is.check("mousse::memInfo::memInfo(mousse::Istream&)");
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, memInfo&)
{
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>(mousse::Istream&, mousse::memInfo&)"
  );
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const memInfo&)
{
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::memInfo&)"
  );
  return os;
}


// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
#include "iostreams.hpp"
#include "constants.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(specie, 0);

}


// Constructors 
mousse::specie::specie(Istream& is)
:
  name_{is},
  nMoles_{readScalar(is)},
  molWeight_{readScalar(is)}
{
  is.check("specie::specie(Istream& is)");
}


mousse::specie::specie(const dictionary& dict)
:
  name_{dict.dictName()},
  nMoles_{readScalar(dict.subDict("specie").lookup("nMoles"))},
  molWeight_{readScalar(dict.subDict("specie").lookup("molWeight"))}
{}


// Member Functions 
void mousse::specie::write(Ostream& os) const
{
  dictionary dict("specie");
  dict.add("nMoles", nMoles_);
  dict.add("molWeight", molWeight_);
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const specie& st)
{
  os << st.name_ << tab
    << st.nMoles_ << tab
    << st.molWeight_;
  os.check("Ostream& operator<<(Ostream& os, const specie& st)");
  return os;
}


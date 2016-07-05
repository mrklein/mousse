// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_const.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Specie>
mousse::rhoConst<Specie>::rhoConst(Istream& is)
:
  Specie{is},
  rho_{readScalar(is)}
{
  is.check("rhoConst<Specie>::rhoConst(Istream& is)");
}


template<class Specie>
mousse::rhoConst<Specie>::rhoConst(const dictionary& dict)
:
  Specie{dict},
  rho_{readScalar(dict.subDict("equationOfState").lookup("rho"))}
{}


// Member Functions 
template<class Specie>
void mousse::rhoConst<Specie>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add("rho", rho_);
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<(Ostream& os, const rhoConst<Specie>& ico)
{
  os << static_cast<const Specie&>(ico)
    << token::SPACE << ico.rho_;
  os.check("Ostream& operator<<(Ostream& os, const rhoConst<Specie>& ico)");
  return os;
}


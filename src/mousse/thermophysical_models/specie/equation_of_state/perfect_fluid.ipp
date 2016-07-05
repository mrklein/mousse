// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "perfect_fluid.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Specie>
mousse::perfectFluid<Specie>::perfectFluid(Istream& is)
:
  Specie{is},
  R_{readScalar(is)},
  rho0_{readScalar(is)}
{
  is.check("perfectFluid<Specie>::perfectFluid(Istream& is)");
}


template<class Specie>
mousse::perfectFluid<Specie>::perfectFluid(const dictionary& dict)
:
  Specie{dict},
  R_{readScalar(dict.subDict("equationOfState").lookup("R"))},
  rho0_{readScalar(dict.subDict("equationOfState").lookup("rho0"))}
{}


// Member Functions 
template<class Specie>
void mousse::perfectFluid<Specie>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add("R", R_);
  dict.add("rho0", rho0_);
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<(Ostream& os, const perfectFluid<Specie>& pf)
{
  os << static_cast<const Specie&>(pf)
    << token::SPACE << pf.R_
    << token::SPACE << pf.rho0_;
  os.check("Ostream& operator<<(Ostream&, const perfectFluid<Specie>&)");
  return os;
}


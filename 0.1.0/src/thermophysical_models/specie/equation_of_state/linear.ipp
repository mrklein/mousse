// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Specie>
mousse::linear<Specie>::linear(Istream& is)
:
  Specie{is},
  psi_{readScalar(is}),
  rho0_{readScalar(is})
{
  is.check("linear<Specie>::linear(Istream& is)");
}


template<class Specie>
mousse::linear<Specie>::linear(const dictionary& dict)
:
  Specie{dict},
  psi_{readScalar(dict.subDict("equationOfState"}.lookup("psi"))),
  rho0_{readScalar(dict.subDict("equationOfState"}.lookup("rho0")))
{}


// Member Functions 
template<class Specie>
void mousse::linear<Specie>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add("psi", psi_);
  dict.add("rho0", rho0_);
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<(Ostream& os, const linear<Specie>& pf)
{
  os << static_cast<const Specie&>(pf)
    << token::SPACE << pf.psi_
    << token::SPACE << pf.rho0_;
  os.check("Ostream& operator<<(Ostream&, const linear<Specie>&)");
  return os;
}


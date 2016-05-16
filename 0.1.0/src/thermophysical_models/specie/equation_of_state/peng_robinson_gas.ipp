// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "peng_robinson_gas.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Specie>
mousse::PengRobinsonGas<Specie>::PengRobinsonGas(Istream& is)
:
  Specie{is},
  Tc_{readScalar(is)},
  Vc_{readScalar(is)},
  Zc_{readScalar(is)},
  Pc_{readScalar(is)},
  omega_{readScalar(is)}
{
  is.check("PengRobinsonGas<Specie>::PengRobinsonGas(Istream& is)");
}


template<class Specie>
mousse::PengRobinsonGas<Specie>::PengRobinsonGas
(
  const dictionary& dict
)
:
  Specie{dict},
  Tc_{readScalar(dict.subDict("equationOfState").lookup("Tc"))},
  Vc_{readScalar(dict.subDict("equationOfState").lookup("Vc"))},
  Zc_{1.0},
  Pc_{readScalar(dict.subDict("equationOfState").lookup("Pc"))},
  omega_{readScalar(dict.subDict("equationOfState").lookup("omega"))}
{
  Zc_ = Pc_*Vc_/(RR*Tc_);
}


// Member Functions 
template<class Specie>
void mousse::PengRobinsonGas<Specie>::write(Ostream& os) const
{
  Specie::write(os);
}


// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const PengRobinsonGas<Specie>& pg
)
{
  os << static_cast<const Specie&>(pg)
    << token::SPACE << pg.Tc_
    << token::SPACE << pg.Vc_
    << token::SPACE << pg.Zc_
    << token::SPACE << pg.Pc_
    << token::SPACE << pg.omega_;
  os.check
  (
    "Ostream& operator<<(Ostream& os, const PengRobinsonGas<Specie>& st)"
  );
  return os;
}


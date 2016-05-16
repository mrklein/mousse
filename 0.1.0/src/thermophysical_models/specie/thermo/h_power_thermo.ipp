// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h_power_thermo.hpp"
#include "iostreams.hpp"


// Constructors 
template<class EquationOfState>
mousse::hPowerThermo<EquationOfState>::hPowerThermo(Istream& is)
:
  EquationOfState{is},
  n0_{readScalar(is)},
  Tref_{readScalar(is)},
  Hf_{readScalar(is)}
{
  is.check("hPowerThermo::hPowerThermo(Istream& is)");
  c0_ *= this->W();
  Hf_ *= this->W();
}


template<class EquationOfState>
mousse::hPowerThermo<EquationOfState>::hPowerThermo
(
  const dictionary& dict
)
:
  EquationOfState{dict},
  c0_{readScalar(dict.subDict("thermodynamics").lookup("C0"))},
  n0_{readScalar(dict.subDict("thermodynamics").lookup("n0"))},
  Tref_{readScalar(dict.subDict("thermodynamics").lookup("Tref"))},
  Hf_{readScalar(dict.subDict("thermodynamics").lookup("Hf"))}
{
  c0_ *= this->W();
  Hf_ *= this->W();
}


// Ostream Operator 
template<class EquationOfState>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const hPowerThermo<EquationOfState>& et
)
{
  os << static_cast<const EquationOfState&>(et) << nl
    << "    " << et.c0_
    << tab << et.n0_
    << tab << et.Tref_
    << tab << et.Hf_;
  os << nl << "    ";
  os << endl;
  os.check
  (
    "operator<<(Ostream& os, const hPowerThermo<EquationOfState>& et)"
  );
  return os;
}


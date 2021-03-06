// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h_const_thermo.hpp"
#include "iostreams.hpp"


// Constructors 
template<class EquationOfState>
mousse::hConstThermo<EquationOfState>::hConstThermo(Istream& is)
:
  EquationOfState{is},
  Cp_{readScalar(is)},
  Hf_{readScalar(is)}
{
  is.check("hConstThermo::hConstThermo(Istream& is)");
  Cp_ *= this->W();
  Hf_ *= this->W();
}


template<class EquationOfState>
mousse::hConstThermo<EquationOfState>::hConstThermo(const dictionary& dict)
:
  EquationOfState{dict},
  Cp_{readScalar(dict.subDict("thermodynamics").lookup("Cp"))},
  Hf_{readScalar(dict.subDict("thermodynamics").lookup("Hf"))}
{
  Cp_ *= this->W();
  Hf_ *= this->W();
}


// Member Functions 
template<class EquationOfState>
void mousse::hConstThermo<EquationOfState>::write(Ostream& os) const
{
  EquationOfState::write(os);
  dictionary dict{"thermodynamics"};
  dict.add("Cp", Cp_/this->W());
  dict.add("Hf", Hf_/this->W());
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
template<class EquationOfState>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const hConstThermo<EquationOfState>& ct
)
{
  os << static_cast<const EquationOfState&>(ct) << tab
    << ct.Cp_/ct.W() << tab << ct.Hf_/ct.W();
  os.check("Ostream& operator<<(Ostream& os, const hConstThermo& ct)");
  return os;
}


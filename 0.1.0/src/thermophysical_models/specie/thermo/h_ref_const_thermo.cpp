// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h_ref_const_thermo.hpp"
#include "iostreams.hpp"
// Constructors 
template<class EquationOfState>
mousse::hRefConstThermo<EquationOfState>::hRefConstThermo(Istream& is)
:
  EquationOfState(is),
  Cp_(readScalar(is)),
  Hf_(readScalar(is)),
  Tref_(readScalar(is)),
  Href_(readScalar(is))
{
  is.check("hRefConstThermo::hRefConstThermo(Istream& is)");
  Cp_ *= this->W();
  Hf_ *= this->W();
  Href_ *= this->W();
}
template<class EquationOfState>
mousse::hRefConstThermo<EquationOfState>::hRefConstThermo(const dictionary& dict)
:
  EquationOfState(dict),
  Cp_(readScalar(dict.subDict("thermodynamics").lookup("Cp"))),
  Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf"))),
  Tref_(readScalar(dict.subDict("thermodynamics").lookup("Tref"))),
  Href_(readScalar(dict.subDict("thermodynamics").lookup("Href")))
{
  Cp_ *= this->W();
  Hf_ *= this->W();
  Href_ *= this->W();
}
// Member Functions 
template<class EquationOfState>
void mousse::hRefConstThermo<EquationOfState>::write(Ostream& os) const
{
  EquationOfState::write(os);
  dictionary dict("thermodynamics");
  dict.add("Cp", Cp_/this->W());
  dict.add("Hf", Hf_/this->W());
  dict.add("Tref", Tref_);
  dict.add("Href", Href_/this->W());
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class EquationOfState>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const hRefConstThermo<EquationOfState>& ct
)
{
  os  << static_cast<const EquationOfState&>(ct) << tab
    << ct.Cp_/ct.W() << tab << ct.Hf_/ct.W() << tab
    << ct.Tref_ << tab << ct.Href_/ct.W();
  os.check("Ostream& operator<<(Ostream& os, const hRefConstThermo& ct)");
  return os;
}

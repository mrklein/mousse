// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "e_const_thermo.hpp"
#include "iostreams.hpp"
// Constructors 
template<class EquationOfState>
mousse::eConstThermo<EquationOfState>::eConstThermo(Istream& is)
:
  EquationOfState(is),
  Cv_(readScalar(is)),
  Hf_(readScalar(is))
{
  is.check("eConstThermo<EquationOfState>::eConstThermo(Istream&)");
  Cv_ *= this->W();
  Hf_ *= this->W();
}
template<class EquationOfState>
mousse::eConstThermo<EquationOfState>::eConstThermo(const dictionary& dict)
:
  EquationOfState(dict),
  Cv_(readScalar(dict.subDict("thermodynamics").lookup("Cv"))),
  Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{
  Cv_ *= this->W();
  Hf_ *= this->W();
}
// Member Functions 
template<class EquationOfState>
void mousse::eConstThermo<EquationOfState>::write(Ostream& os) const
{
  EquationOfState::write(os);
  dictionary dict("thermodynamics");
  dict.add("Cv", Cv_/this->W());
  dict.add("Hf", Hf_/this->W());
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class EquationOfState>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const eConstThermo<EquationOfState>& ct
)
{
  os  << static_cast<const EquationOfState&>(ct) << tab
    << ct.Cv_/ct.W() << tab << ct.Hf_/ct.W();
  os.check("Ostream& operator<<(Ostream&, const eConstThermo&)");
  return os;
}

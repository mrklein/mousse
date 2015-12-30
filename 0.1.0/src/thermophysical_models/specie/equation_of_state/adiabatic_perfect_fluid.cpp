// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adiabatic_perfect_fluid.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Specie>
mousse::adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid(Istream& is)
:
  Specie(is),
  p0_(readScalar(is)),
  rho0_(readScalar(is)),
  gamma_(readScalar(is)),
  B_(readScalar(is))
{
  is.check
  (
    "adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid(Istream& is)"
  );
}
template<class Specie>
mousse::adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid
(
  const dictionary& dict
)
:
  Specie(dict),
  p0_(readScalar(dict.subDict("equationOfState").lookup("p0"))),
  rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0"))),
  gamma_(readScalar(dict.subDict("equationOfState").lookup("gamma"))),
  B_(readScalar(dict.subDict("equationOfState").lookup("B")))
{}
// Member Functions 
template<class Specie>
void mousse::adiabaticPerfectFluid<Specie>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add("p0", p0_);
  dict.add("rho0", rho0_);
  dict.add("gamma", gamma_);
  dict.add("B", B_);
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const adiabaticPerfectFluid<Specie>& pf
)
{
  os  << static_cast<const Specie&>(pf)
    << token::SPACE << pf.R_
    << token::SPACE << pf.rho0_
    << token::SPACE << pf.gamma_
    << token::SPACE << pf.B_;
  os.check
  (
    "Ostream& operator<<(Ostream&, const adiabaticPerfectFluid<Specie>&)"
  );
  return os;
}

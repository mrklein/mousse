// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_perfect_gas.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Specie>
mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas(Istream& is)
:
  Specie(is),
  pRef_(readScalar(is))
{
  is.check
  (
    "incompressiblePerfectGas<Specie>::"
    "incompressiblePerfectGas(Istream& is)"
  );
}
template<class Specie>
mousse::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
  const dictionary& dict
)
:
  Specie(dict),
  pRef_(readScalar(dict.subDict("equationOfState").lookup("pRef")))
{}
// Member Functions 
template<class Specie>
void mousse::incompressiblePerfectGas<Specie>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add("pRef", pRef_);
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const incompressiblePerfectGas<Specie>& pg
)
{
  os  << static_cast<const Specie&>(pg)
    << token::SPACE << pg.pRef_;
  os.check
  (
    "Ostream& operator<<"
    "(Ostream& os, const incompressiblePerfectGas<Specie>& st)"
  );
  return os;
}

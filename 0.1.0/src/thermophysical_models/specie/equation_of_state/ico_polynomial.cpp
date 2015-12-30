// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ico_polynomial.hpp"
#include "iostreams.hpp"
namespace mousse
{
// Constructors 
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize>::icoPolynomial(Istream& is)
:
  Specie(is),
  rhoCoeffs_("rhoCoeffs<" + mousse::name(PolySize) + '>', is)
{
  rhoCoeffs_ *= this->W();
}
template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize>::icoPolynomial(const dictionary& dict)
:
  Specie(dict),
  rhoCoeffs_
(
  dict.subDict("equationOfState").lookup
  (
    "rhoCoeffs<" + mousse::name(PolySize) + '>'
  )
)
{
  rhoCoeffs_ *= this->W();
}
// Member Functions 
template<class Specie, int PolySize>
void icoPolynomial<Specie, PolySize>::write(Ostream& os) const
{
  Specie::write(os);
  dictionary dict("equationOfState");
  dict.add
  (
    word("rhoCoeffs<" + mousse::name(PolySize) + '>'),
    rhoCoeffs_/this->W()
  );
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class Specie, int PolySize>
Ostream& operator<<(Ostream& os, const icoPolynomial<Specie, PolySize>& ip)
{
  os  << static_cast<const Specie&>(ip) << tab
    << "rhoCoeffs<" << mousse::name(PolySize) << '>' << tab
    << ip.rhoCoeffs_/ip.W();
  os.check
  (
    "Ostream& operator<<"
    "(Ostream& os, const icoPolynomial<Specie, PolySize>& ip)"
  );
  return os;
}
}  // namespace mousse

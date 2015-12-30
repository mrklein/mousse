// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial_solid_transport.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Thermo, int PolySize>
mousse::polynomialSolidTransport<Thermo, PolySize>::polynomialSolidTransport
(
  Istream& is
)
:
  Thermo(is),
  kappaCoeffs_("kappaCoeffs<" + mousse::name(PolySize) + '>', is)
{
}
template<class Thermo, int PolySize>
mousse::polynomialSolidTransport<Thermo, PolySize>::polynomialSolidTransport
(
  const dictionary& dict
)
:
  Thermo(dict),
  kappaCoeffs_
  (
    dict.subDict("transport").lookup
    (
      "kappaCoeffs<" + mousse::name(PolySize) + '>'
    )
  )
{
}
// Member Functions 
template<class Thermo, int PolySize>
void mousse::polynomialSolidTransport<Thermo, PolySize>::write(Ostream& os) const
{
  Thermo::write(os);
  dictionary dict("transport");
  dict.add
  (
    word("kappaCoeffs<" + mousse::name(PolySize) + '>'),
    kappaCoeffs_
  );
  os  << indent << dict.dictName() << dict;
}
// IOstream Operators 
template<class Thermo, int PolySize>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
  os  << static_cast<const Thermo&>(pt) << tab
    << "kappaCoeffs<" << mousse::name(PolySize) << '>' << tab
    << pt.kappaCoeffs_;
  os.check
  (
    "Ostream& operator<<"
    "("
      "Ostream&, "
      "const polynomialSolidTransport<Thermo, PolySize>&"
    ")"
  );
  return os;
}

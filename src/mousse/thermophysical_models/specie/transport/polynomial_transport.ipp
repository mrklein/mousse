// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial_transport.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Thermo, int PolySize>
mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport(Istream& is)
:
  Thermo{is},
  muCoeffs_{"muCoeffs<" + mousse::name(PolySize) + '>', is},
  kappaCoeffs_{"kappaCoeffs<" + mousse::name(PolySize) + '>', is}
{
  muCoeffs_ *= this->W();
  kappaCoeffs_ *= this->W();
}


template<class Thermo, int PolySize>
mousse::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
  const dictionary& dict
)
:
  Thermo{dict},
  muCoeffs_
  {
    dict.subDict("transport").lookup
    (
      "muCoeffs<" + mousse::name(PolySize) + '>'
    )
  },
  kappaCoeffs_
  {
    dict.subDict("transport").lookup
    (
      "kappaCoeffs<" + mousse::name(PolySize) + '>'
    )
  }
{
  muCoeffs_ *= this->W();
  kappaCoeffs_ *= this->W();
}


// Member Functions 
template<class Thermo, int PolySize>
void mousse::polynomialTransport<Thermo, PolySize>::write(Ostream& os) const
{
  os << this->name() << endl;
  os << token::BEGIN_BLOCK << incrIndent << nl;
  Thermo::write(os);
  dictionary dict{"transport"};
  dict.add
  (
    word("muCoeffs<" + mousse::name(PolySize) + '>'),
    muCoeffs_/this->W()
  );
  dict.add
  (
    word("kappaCoeffs<" + mousse::name(PolySize) + '>'),
    kappaCoeffs_/this->W()
  );
  os << indent << dict.dictName() << dict;
  os << decrIndent << token::END_BLOCK << nl;
}


// IOstream Operators 
template<class Thermo, int PolySize>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const polynomialTransport<Thermo, PolySize>& pt
)
{
  os << static_cast<const Thermo&>(pt) << tab
    << "muCoeffs<" << mousse::name(PolySize) << '>' << tab
    << pt.muCoeffs_/pt.W() << tab
    << "kappaCoeffs<" << mousse::name(PolySize) << '>' << tab
    << pt.kappaCoeffs_/pt.W();
  os.check
  (
    "Ostream& operator<<"
    "("
    "  Ostream&,"
    "  const polynomialTransport<Thermo, PolySize>&"
    ")"
  );
  return os;
}


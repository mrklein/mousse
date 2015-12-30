// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "const_transport.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Thermo>
mousse::constTransport<Thermo>::constTransport(Istream& is)
:
  Thermo(is),
  mu_(readScalar(is)),
  rPr_(1.0/readScalar(is))
{
  is.check("constTransport::constTransport(Istream& is)");
}
template<class Thermo>
mousse::constTransport<Thermo>::constTransport(const dictionary& dict)
:
  Thermo(dict),
  mu_(readScalar(dict.subDict("transport").lookup("mu"))),
  rPr_(1.0/readScalar(dict.subDict("transport").lookup("Pr")))
{}
// Member Functions 
template<class Thermo>
void mousse::constTransport<Thermo>::constTransport::write(Ostream& os) const
{
  os  << this->name() << endl;
  os  << token::BEGIN_BLOCK  << incrIndent << nl;
  Thermo::write(os);
  dictionary dict("transport");
  dict.add("mu", mu_);
  dict.add("Pr", 1.0/rPr_);
  os  << indent << dict.dictName() << dict;
  os  << decrIndent << token::END_BLOCK << nl;
}
// IOstream Operators 
template<class Thermo>
mousse::Ostream& mousse::operator<<(Ostream& os, const constTransport<Thermo>& ct)
{
  operator<<(os, static_cast<const Thermo&>(ct));
  os << tab << ct.mu_ << tab << 1.0/ct.rPr_;
  os.check("Ostream& operator<<(Ostream&, const constTransport&)");
  return os;
}

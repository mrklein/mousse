// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sutherland_transport.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Thermo>
mousse::sutherlandTransport<Thermo>::sutherlandTransport(Istream& is)
:
  Thermo(is),
  As_(readScalar(is)),
  Ts_(readScalar(is))
{
  is.check("sutherlandTransport<Thermo>::sutherlandTransport(Istream&)");
}
template<class Thermo>
mousse::sutherlandTransport<Thermo>::sutherlandTransport(const dictionary& dict)
:
  Thermo(dict),
  As_(readScalar(dict.subDict("transport").lookup("As"))),
  Ts_(readScalar(dict.subDict("transport").lookup("Ts")))
{}
// Member Functions 
template<class Thermo>
void mousse::sutherlandTransport<Thermo>::write(Ostream& os) const
{
  os  << this->specie::name() << endl;
  os  << token::BEGIN_BLOCK  << incrIndent << nl;
  Thermo::write(os);
  dictionary dict("transport");
  dict.add("As", As_);
  dict.add("Ts", Ts_);
  os  << indent << dict.dictName() << dict;
  os  << decrIndent << token::END_BLOCK << nl;
}
// IOstream Operators 
template<class Thermo>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const sutherlandTransport<Thermo>& st
)
{
  os << static_cast<const Thermo&>(st) << tab << st.As_ << tab << st.Ts_;
  os.check
  (
    "Ostream& operator<<(Ostream&, const sutherlandTransport<Thermo>&)"
  );
  return os;
}

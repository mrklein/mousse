// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "const_an_iso_solid_transport.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Thermo>
mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
  const dictionary& dict
)
:
  Thermo(dict),
  kappa_(dict.subDict("transport").lookup("kappa"))
{}
// Member Functions 
template<class Thermo>
void mousse::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport::write
(
  Ostream& os
) const
{
  Thermo::write(os);
  dictionary dict("transport");
  dict.add("kappa", kappa_);
  os  << indent << dict.dictName() << dict;
}
// IOstream Operators 
template<class Thermo>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const constAnIsoSolidTransport<Thermo>& ct
)
{
  operator<<(os, static_cast<const Thermo&>(ct));
  os << tab << ct.kappa_;
  os.check
  (
    "Ostream& operator<<(Ostream& os,const constAnIsoSolidTransport& ct)"
  );
  return os;
}

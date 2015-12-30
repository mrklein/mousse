// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "const_iso_solid_transport.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Thermo>
mousse::constIsoSolidTransport<Thermo>::constIsoSolidTransport
(
  const dictionary& dict
)
:
  Thermo(dict),
  kappa_(readScalar(dict.subDict("transport").lookup("kappa")))
{}
// Member Functions 
template<class Thermo>
void mousse::constIsoSolidTransport<Thermo>::constIsoSolidTransport::write
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
  const constIsoSolidTransport<Thermo>& ct
)
{
  operator<<(os, static_cast<const Thermo&>(ct));
  os << tab << ct.kappa_;
  os.check
  (
    "Ostream& operator<<(Ostream& os,const constIsoSolidTransport& ct)"
  );
  return os;
}

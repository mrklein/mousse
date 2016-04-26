// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "exponential_solid_transport.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Thermo>
mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport
(
  const dictionary& dict
)
:
  Thermo{dict},
  kappa0_{0.0},
  n0_{0.0},
  Tref_{0.0}
{
  const dictionary& subDict = dict.subDict("transport");
  kappa0_ = readScalar(subDict.lookup("kappa0"));
  n0_ = readScalar(subDict.lookup("n0"));
  Tref_ = readScalar(subDict.lookup("Tref"));
}


// Member Functions 
template<class Thermo>
void mousse::exponentialSolidTransport<Thermo>::exponentialSolidTransport::write
(
  Ostream& os
) const
{
  Thermo::write(os);
  dictionary dict{"transport"};
  dict.add("kappa0", kappa0_);
  dict.add("n0", n0_);
  dict.add("Tref", Tref_);
  os << indent << dict.dictName() << dict;
}


// IOstream Operators 
template<class Thermo>
mousse::Ostream& mousse::operator<<
(
  Ostream& os, const exponentialSolidTransport<Thermo>& et
)
{
  operator<<(os, static_cast<const Thermo&>(et));
  os << tab << et.kappa0_  << tab << et.n0_ << tab << et.Tref_;
  os.check
  (
    "Ostream& operator<<(Ostream& os, const exponentialSolidTransport& et)"
  );
  return os;
}


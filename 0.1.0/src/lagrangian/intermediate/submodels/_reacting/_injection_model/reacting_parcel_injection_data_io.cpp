// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_parcel_injection_data.hpp"
mousse::reactingParcelInjectionData::reactingParcelInjectionData(Istream& is)
:
  thermoParcelInjectionData(is)
{
  is.check("reading Y's");
  is >> Y_;
  is.check("reactingParcelInjectionData(Istream& is)");
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const reactingParcelInjectionData& data
)
{
  os << static_cast<const thermoParcelInjectionData&>(data);
  os << data.Y_;
  return os;
}
mousse::Istream& mousse::operator>>(Istream& is, reactingParcelInjectionData& data)
{
  is >> static_cast<thermoParcelInjectionData&>(data);
  is.check("reading Y's");
  is >> data.Y_;
  is.check("operator(Istream&, reactingParcelInjectionData&)");
  return is;
}

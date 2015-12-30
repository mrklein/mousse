// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_parcel_injection_data.hpp"
mousse::thermoParcelInjectionData::thermoParcelInjectionData(Istream& is)
:
  kinematicParcelInjectionData(is)
{
  is.check("reading T");
  is >> T_;
  is.check("reading Cp");
  is >> Cp_;
  is.check("thermoParcelInjectionData(Istream& is)");
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const thermoParcelInjectionData& data
)
{
  os << static_cast<const kinematicParcelInjectionData&>(data);
  os << data.T_ << data.Cp_;
  return os;
}
mousse::Istream& mousse::operator>>(Istream& is, thermoParcelInjectionData& data)
{
  is >> static_cast<kinematicParcelInjectionData&>(data);
  is.check("reading T");
  is >> data.T_;
  is.check("reading Cp");
  is >> data.Cp_;
  is.check("operator(Istream&, thermoParcelInjectionData&)");
  return is;
}

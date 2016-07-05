// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_multiphase_parcel_injection_data.hpp"


mousse::reactingMultiphaseParcelInjectionData::
reactingMultiphaseParcelInjectionData(Istream& is)
:
  reactingParcelInjectionData{is}
{
  is.check("reading YGas's");
  is >> YGas_;
  is.check("reading YLiquid's");
  is >> YLiquid_;
  is.check("reading YSolid's");
  is >> YSolid_;
  is.check("reactingMultiphaseParcelInjectionData(Istream& is)");
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const reactingMultiphaseParcelInjectionData& data
)
{
  os << static_cast<const reactingParcelInjectionData&>(data);
  os << data.YGas_ << data.YLiquid_ << data.YSolid_;
  return os;
}


mousse::Istream& mousse::operator>>
(
  Istream& is,
  reactingMultiphaseParcelInjectionData& data
)
{
  is >> static_cast<reactingParcelInjectionData&>(data);
  is.check("reading YGas's");
  is >> data.YGas_;
  is.check("reading YLiquid's");
  is >> data.YLiquid_;
  is.check("reading YSolid's");
  is >> data.YSolid_;
  is.check("operator(Istream&, reactingMultiphaseParcelInjectionData&)");
  return is;
}


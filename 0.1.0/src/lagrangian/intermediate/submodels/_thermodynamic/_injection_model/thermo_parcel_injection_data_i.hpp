// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_parcel_injection_data.hpp"
inline mousse::scalar mousse::thermoParcelInjectionData::T() const
{
  return T_;
}
inline mousse::scalar mousse::thermoParcelInjectionData::Cp() const
{
  return Cp_;
}
inline mousse::scalar& mousse::thermoParcelInjectionData::T()
{
  return T_;
}
inline mousse::scalar& mousse::thermoParcelInjectionData::Cp()
{
  return Cp_;
}

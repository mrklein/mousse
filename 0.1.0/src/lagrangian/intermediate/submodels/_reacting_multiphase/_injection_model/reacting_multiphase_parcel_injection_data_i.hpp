// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_multiphase_parcel_injection_data.hpp"
inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YGas() const
{
  return YGas_;
}
inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YLiquid() const
{
  return YLiquid_;
}
inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YSolid() const
{
  return YSolid_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YGas()
{
  return YGas_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YLiquid()
{
  return YLiquid_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YSolid()
{
  return YSolid_;
}

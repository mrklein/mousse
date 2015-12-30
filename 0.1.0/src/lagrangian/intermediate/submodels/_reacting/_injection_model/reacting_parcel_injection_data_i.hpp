// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_parcel_injection_data.hpp"
inline const mousse::scalarList& mousse::reactingParcelInjectionData::Y() const
{
  return Y_;
}
inline mousse::scalarList& mousse::reactingParcelInjectionData::Y()
{
  return Y_;
}

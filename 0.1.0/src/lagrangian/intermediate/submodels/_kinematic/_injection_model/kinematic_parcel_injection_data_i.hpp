// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kinematic_parcel_injection_data.hpp"
inline const mousse::point& mousse::kinematicParcelInjectionData::x() const
{
  return x_;
}
inline const mousse::vector& mousse::kinematicParcelInjectionData::U() const
{
  return U_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::d() const
{
  return d_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::rho() const
{
  return rho_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::mDot() const
{
  return mDot_;
}
inline mousse::point& mousse::kinematicParcelInjectionData::x()
{
  return x_;
}
inline mousse::vector& mousse::kinematicParcelInjectionData::U()
{
  return U_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::d()
{
  return d_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::rho()
{
  return rho_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::mDot()
{
  return mDot_;
}

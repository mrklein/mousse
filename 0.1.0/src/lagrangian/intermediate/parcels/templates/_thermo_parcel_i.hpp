// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties()
:
  ParcelType::constantProperties(),
  T0_(this->dict_, 0.0),
  TMin_(this->dict_, 0.0),
  TMax_(this->dict_, VGREAT),
  Cp0_(this->dict_, 0.0),
  epsilon0_(this->dict_, 0.0),
  f0_(this->dict_, 0.0)
{}
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties(cp),
  T0_(cp.T0_),
  TMin_(cp.TMin_),
  TMax_(cp.TMax_),
  Cp0_(cp.Cp0_),
  epsilon0_(cp.epsilon0_),
  f0_(cp.f0_)
{}
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties(parentDict),
  T0_(this->dict_, "T0"),
  TMin_(this->dict_, "TMin", 200.0),
  TMax_(this->dict_, "TMax", 5000.0),
  Cp0_(this->dict_, "Cp0"),
  epsilon0_(this->dict_, "epsilon0"),
  f0_(this->dict_, "f0")
{}
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
  T_(0.0),
  Cp_(0.0),
  Tc_(0.0),
  Cpc_(0.0)
{}
template<class ParcelType>
inline mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const constantProperties& constProps
)
:
  ParcelType
  (
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    f0,
    angularMomentum0,
    torque0,
    constProps
  ),
  T_(constProps.T0()),
  Cp_(constProps.Cp0()),
  Tc_(0.0),
  Cpc_(0.0)
{}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::T0() const
{
  return T0_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::TMin() const
{
  return TMin_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::TMax() const
{
  return TMax_.value();
}
template<class ParcelType>
inline void
mousse::ThermoParcel<ParcelType>::constantProperties::setTMax(const scalar TMax)
{
  TMax_.setValue(TMax);
}
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::Cp0() const
{
  return Cp0_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::epsilon0() const
{
  return epsilon0_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ThermoParcel<ParcelType>::constantProperties::f0() const
{
  return f0_.value();
}
// ThermoParcel Member Functions
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::T() const
{
  return T_;
}
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Cp() const
{
  return Cp_;
}
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::hs() const
{
  return Cp_*(T_ - 298.15);
}
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Tc() const
{
  return Tc_;
}
template<class ParcelType>
inline mousse::scalar mousse::ThermoParcel<ParcelType>::Cpc() const
{
  return Cpc_;
}
template<class ParcelType>
inline mousse::scalar& mousse::ThermoParcel<ParcelType>::T()
{
  return T_;
}
template<class ParcelType>
inline mousse::scalar& mousse::ThermoParcel<ParcelType>::Cp()
{
  return Cp_;
}

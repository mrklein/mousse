// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_step_reacting_mixture.hpp"
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::stoicRatio() const
{
  return stoicRatio_;
}
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::s() const
{
  return s_;
}
template<class ThermoType>
inline const mousse::dimensionedScalar
mousse::singleStepReactingMixture<ThermoType>::qFuel() const
{
  return qFuel_;
}
template<class ThermoType>
inline const mousse::List<mousse::scalar>&
mousse::singleStepReactingMixture<ThermoType>::specieStoichCoeffs() const
{
  return specieStoichCoeffs_;
}
template<class ThermoType>
inline mousse::tmp<mousse::volScalarField>
mousse::singleStepReactingMixture<ThermoType>::fres
(
  const label index
) const
{
  return fres_[index];
}
template<class ThermoType>
inline mousse::label
mousse::singleStepReactingMixture<ThermoType>::inertIndex() const
{
  return inertIndex_;
}
template<class ThermoType>
inline mousse::label
mousse::singleStepReactingMixture<ThermoType>::fuelIndex() const
{
  return fuelIndex_;
}
template<class ThermoType>
inline const mousse::List<int>&
mousse::singleStepReactingMixture<ThermoType>::specieProd() const
{
  return specieProd_;
}
template<class ThermoType>
inline const mousse::scalarList&
mousse::singleStepReactingMixture<ThermoType>::Yprod0() const
{
  return Yprod0_;
}

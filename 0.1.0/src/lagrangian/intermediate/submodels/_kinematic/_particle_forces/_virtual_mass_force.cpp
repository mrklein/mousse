// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_virtual_mass_force.hpp"
// Constructors 
template<class CloudType>
mousse::VirtualMassForce<CloudType>::VirtualMassForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType
)
:
  PressureGradientForce<CloudType>(owner, mesh, dict, forceType),
  Cvm_(readScalar(this->coeffs().lookup("Cvm")))
{}
template<class CloudType>
mousse::VirtualMassForce<CloudType>::VirtualMassForce
(
  const VirtualMassForce& vmf
)
:
  PressureGradientForce<CloudType>(vmf),
  Cvm_(vmf.Cvm_)
{}
// Destructor 
template<class CloudType>
mousse::VirtualMassForce<CloudType>::~VirtualMassForce()
{}
// Member Functions 
template<class CloudType>
void mousse::VirtualMassForce<CloudType>::cacheFields(const bool store)
{
  PressureGradientForce<CloudType>::cacheFields(store);
}
template<class CloudType>
mousse::forceSuSp mousse::VirtualMassForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value =
    PressureGradientForce<CloudType>::calcCoupled(p, dt, mass, Re, muc);
  value.Su() *= Cvm_;
  return value;
}
template<class CloudType>
mousse::scalar mousse::VirtualMassForce<CloudType>::massAdd
(
  const typename CloudType::parcelType& p,
  const scalar mass
) const
{
  return mass*p.rhoc()/p.rho()*Cvm_;
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_non_sphere_drag_force.hpp"
// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::NonSphereDragForce<CloudType>::CdRe(const scalar Re) const
{
  return 24.0*(1.0 + a_*pow(Re, b_)) + Re*c_/(1 + d_/(Re + ROOTVSMALL));
}
// Constructors 
template<class CloudType>
mousse::NonSphereDragForce<CloudType>::NonSphereDragForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
  phi_(readScalar(this->coeffs().lookup("phi"))),
  a_(exp(2.3288 - 6.4581*phi_ + 2.4486*sqr(phi_))),
  b_(0.0964 + 0.5565*phi_),
  c_(exp(4.9050 - 13.8944*phi_ + 18.4222*sqr(phi_) - 10.2599*pow3(phi_))),
  d_(exp(1.4681 + 12.2584*phi_ - 20.7322*sqr(phi_) + 15.8855*pow3(phi_)))
{
  if (phi_ <= 0 || phi_ > 1)
  {
    FatalErrorIn
    (
      "NonSphereDrag<CloudType>::NonSphereDrag"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Ratio of surface of sphere having same volume as particle to "
      << "actual surface area of particle (phi) must be greater than 0 "
      << "and less than or equal to 1" << exit(FatalError);
  }
}
template<class CloudType>
mousse::NonSphereDragForce<CloudType>::NonSphereDragForce
(
  const NonSphereDragForce<CloudType>& df
)
:
  ParticleForce<CloudType>(df),
  phi_(df.phi_),
  a_(df.a_),
  b_(df.b_),
  c_(df.c_),
  d_(df.d_)
{}
// Destructor 
template<class CloudType>
mousse::NonSphereDragForce<CloudType>::~NonSphereDragForce()
{}
// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::NonSphereDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  value.Sp() = mass*0.75*muc*CdRe(Re)/(p.rho()*sqr(p.d()));
  return value;
}

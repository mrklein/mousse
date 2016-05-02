// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_gravity_force.hpp"


// Constructors 
template<class CloudType>
mousse::GravityForce<CloudType>::GravityForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>{owner, mesh, dict, typeName, false},
  g_{owner.g().value()}
{}


template<class CloudType>
mousse::GravityForce<CloudType>::GravityForce(const GravityForce& gf)
:
  ParticleForce<CloudType>{gf},
  g_{gf.g_}
{}


// Destructor 
template<class CloudType>
mousse::GravityForce<CloudType>::~GravityForce()
{}


// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::GravityForce<CloudType>::calcNonCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar /*Re*/,
  const scalar /*muc*/
) const
{
  forceSuSp value(vector::zero, 0.0);
  value.Su() = mass*g_*(1.0 - p.rhoc()/p.rho());
  return value;
}


// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_sphere_drag_force.hpp"
// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::SphereDragForce<CloudType>::CdRe(const scalar Re) const
{
  if (Re > 1000.0)
  {
    return 0.424*Re;
  }
  else
  {
    return 24.0*(1.0 + 1.0/6.0*pow(Re, 2.0/3.0));
  }
}
// Constructors 
template<class CloudType>
mousse::SphereDragForce<CloudType>::SphereDragForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>(owner, mesh, dict, typeName, false)
{}
template<class CloudType>
mousse::SphereDragForce<CloudType>::SphereDragForce
(
  const SphereDragForce<CloudType>& df
)
:
  ParticleForce<CloudType>(df)
{}
// Destructor 
template<class CloudType>
mousse::SphereDragForce<CloudType>::~SphereDragForce()
{}
// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::SphereDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  value.Sp() = mass*0.75*muc*CdRe(Re)/(p.rho()*sqr(p.d()));
  return value;
}

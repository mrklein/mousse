// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_distorted_sphere_drag_force.hpp"
// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::DistortedSphereDragForce<CloudType>::CdRe
(
  const scalar Re
) const
{
  if (Re > 1000.0)
  {
    return 0.424*Re;
  }
  else
  {
    return 24.0*(1.0 + (1.0/6.0)*pow(Re, 2.0/3.0));
  }
}
// Constructors 
template<class CloudType>
mousse::DistortedSphereDragForce<CloudType>::DistortedSphereDragForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>(owner, mesh, dict, typeName, false)
{}
template<class CloudType>
mousse::DistortedSphereDragForce<CloudType>::DistortedSphereDragForce
(
  const DistortedSphereDragForce<CloudType>& df
)
:
  ParticleForce<CloudType>(df)
{}
// Destructor 
template<class CloudType>
mousse::DistortedSphereDragForce<CloudType>::~DistortedSphereDragForce()
{}
// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::DistortedSphereDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  // Limit the drop distortion to y=0 (sphere) and y=1 (disk)
  scalar y = min(max(p.y(), 0), 1);
  value.Sp() = mass*0.75*muc*CdRe(Re)*(1 + 2.632*y)/(p.rho()*sqr(p.d()));
  return value;
}

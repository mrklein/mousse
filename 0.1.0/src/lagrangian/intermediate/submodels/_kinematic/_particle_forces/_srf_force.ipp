// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_srf_force.hpp"


// Constructors 
template<class CloudType>
mousse::SRFForce<CloudType>::SRFForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>{owner, mesh, dict, typeName, false},
  srfPtr_{nullptr}
{}


template<class CloudType>
mousse::SRFForce<CloudType>::SRFForce
(
  const SRFForce& srff
)
:
  ParticleForce<CloudType>{srff},
  srfPtr_{nullptr}
{}


// Destructor 
template<class CloudType>
mousse::SRFForce<CloudType>::~SRFForce()
{}


// Member Functions 
template<class CloudType>
void mousse::SRFForce<CloudType>::cacheFields(const bool store)
{
  if (store) {
    const typename SRF::SRFModel& model = this->mesh().template
      lookupObject<SRF::SRFModel>("SRFProperties");
    srfPtr_ = &model;
  } else {
    srfPtr_ = nullptr;
  }
}


template<class CloudType>
mousse::forceSuSp mousse::SRFForce<CloudType>::calcNonCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar /*Re*/,
  const scalar /*muc*/
) const
{
  forceSuSp value{vector::zero, 0.0};
  const typename SRF::SRFModel& srf = *srfPtr_;
  const vector& omega = srf.omega().value();
  const vector& r = p.position();
  // Coriolis and centrifugal acceleration terms
  value.Su() =
    mass*(1.0 - p.rhoc()/p.rho())*(2.0*(p.U() ^ omega) + (omega ^ (r ^ omega)));
  return value;
}


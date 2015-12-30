// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_force.hpp"
// Constructors 
template<class CloudType>
mousse::ParticleForce<CloudType>::ParticleForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType,
  const bool readCoeffs
)
:
  owner_(owner),
  mesh_(mesh),
  coeffs_(readCoeffs ? dict : dictionary::null)
{
  if (readCoeffs && (coeffs_.dictName() != forceType))
  {
    FatalIOErrorIn
    (
      "mousse::ParticleForce<CloudType>::ParticleForce"
      "("
        "CloudType&, "
        "const fvMesh&, "
        "const dictionary&, "
        "const word&, "
        "const bool"
      ")",
      dict
    )   << "Force " << forceType << " must be specified as a dictionary"
      << exit(FatalIOError);
  }
}
template<class CloudType>
mousse::ParticleForce<CloudType>::ParticleForce(const ParticleForce& pf)
:
  owner_(pf.owner_),
  mesh_(pf.mesh_),
  coeffs_(pf.coeffs_)
{}
// Destructor 
template<class CloudType>
mousse::ParticleForce<CloudType>::~ParticleForce()
{}
// Member Functions 
template<class CloudType>
void mousse::ParticleForce<CloudType>::cacheFields(const bool store)
{}
template<class CloudType>
mousse::forceSuSp mousse::ParticleForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType&,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value;
  value.Su() = vector::zero;
  value.Sp() = 0.0;
  return value;
}
template<class CloudType>
mousse::forceSuSp mousse::ParticleForce<CloudType>::calcNonCoupled
(
  const typename CloudType::parcelType&,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value;
  value.Su() = vector::zero;
  value.Sp() = 0.0;
  return value;
}
template<class CloudType>
mousse::scalar mousse::ParticleForce<CloudType>::massAdd
(
  const typename CloudType::parcelType& p,
  const scalar mass
) const
{
  return 0.0;
}
#include "_particle_force_new.cpp"

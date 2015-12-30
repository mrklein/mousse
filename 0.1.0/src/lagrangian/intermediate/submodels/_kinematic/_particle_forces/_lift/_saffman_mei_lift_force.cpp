// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_saffman_mei_lift_force.hpp"
#include "mathematical_constants.hpp"
using namespace mousse::constant;
// Protected Member Functions 
template<class CloudType>
mousse::scalar mousse::SaffmanMeiLiftForce<CloudType>::SaffmanMeiLiftForce::Cl
(
  const typename CloudType::parcelType& p,
  const vector& curlUc,
  const scalar Re,
  const scalar muc
) const
{
  scalar Rew = p.rhoc()*mag(curlUc)*sqr(p.d())/(muc + ROOTVSMALL);
  scalar beta = 0.5*(Rew/(Re + ROOTVSMALL));
  scalar alpha = 0.3314*sqrt(beta);
  scalar f = (1.0 - alpha)*exp(-0.1*Re) + alpha;
  scalar Cld = 0.0;
  if (Re < 40)
  {
    Cld = 6.46*f;
  }
  else
  {
    Cld = 6.46*0.0524*sqrt(beta*Re);
  }
  return 3.0/(mathematical::twoPi*sqrt(Rew + ROOTVSMALL))*Cld;
}
// Constructors 
template<class CloudType>
mousse::SaffmanMeiLiftForce<CloudType>::SaffmanMeiLiftForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType
)
:
  LiftForce<CloudType>(owner, mesh, dict, forceType)
{}
template<class CloudType>
mousse::SaffmanMeiLiftForce<CloudType>::SaffmanMeiLiftForce
(
  const SaffmanMeiLiftForce& lf
)
:
  LiftForce<CloudType>(lf)
{}
// Destructor 
template<class CloudType>
mousse::SaffmanMeiLiftForce<CloudType>::~SaffmanMeiLiftForce()
{}

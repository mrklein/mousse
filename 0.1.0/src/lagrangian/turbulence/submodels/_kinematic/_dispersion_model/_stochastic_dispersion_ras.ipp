// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_stochastic_dispersion_ras.hpp"
#include "constants.hpp"


using namespace mousse::constant::mathematical;


// Constructors 
template<class CloudType>
mousse::StochasticDispersionRAS<CloudType>::StochasticDispersionRAS
(
  const dictionary& dict,
  CloudType& owner
)
:
  DispersionRASModel<CloudType>{dict, owner}
{}


template<class CloudType>
mousse::StochasticDispersionRAS<CloudType>::StochasticDispersionRAS
(
  const StochasticDispersionRAS<CloudType>& dm
)
:
  DispersionRASModel<CloudType>{dm}
{}


// Destructor 
template<class CloudType>
mousse::StochasticDispersionRAS<CloudType>::~StochasticDispersionRAS()
{}


// Member Functions 
template<class CloudType>
mousse::vector mousse::StochasticDispersionRAS<CloudType>::update
(
  const scalar dt,
  const label cellI,
  const vector& U,
  const vector& Uc,
  vector& UTurb,
  scalar& tTurb
)
{
  cachedRandom& rnd = this->owner().rndGen();
  const scalar cps = 0.16432;
  const scalar k = this->kPtr_->internalField()[cellI];
  const scalar epsilon =
    this->epsilonPtr_->internalField()[cellI] + ROOTVSMALL;
  const scalar UrelMag = mag(U - Uc - UTurb);
  const scalar tTurbLoc =
    min(k/epsilon, cps*pow(k, 1.5)/epsilon/(UrelMag + SMALL));
  // Parcel is perturbed by the turbulence
  if (dt < tTurbLoc) {
    tTurb += dt;
    if (tTurb > tTurbLoc) {
      tTurb = 0;
      const scalar sigma = sqrt(2*k/3.0);
      // Calculate a random direction dir distributed uniformly
      // in spherical coordinates
      const scalar theta = rnd.sample01<scalar>()*twoPi;
      const scalar u = 2*rnd.sample01<scalar>() - 1;
      const scalar a = sqrt(1 - sqr(u));
      const vector dir(a*cos(theta), a*sin(theta), u);
      UTurb = sigma*mag(rnd.GaussNormal<scalar>())*dir;
    }
  } else {
    tTurb = GREAT;
    UTurb = vector::zero;
  }
  return Uc + UTurb;
}


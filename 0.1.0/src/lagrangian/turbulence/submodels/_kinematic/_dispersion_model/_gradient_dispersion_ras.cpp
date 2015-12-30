// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_gradient_dispersion_ras.hpp"
#include "demand_driven_data.hpp"
#include "fvc_grad.hpp"

// Constructors 
template<class CloudType>
mousse::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
  const dictionary& dict,
  CloudType& owner
)
:
  DispersionRASModel<CloudType>(dict, owner),
  gradkPtr_(NULL),
  ownGradK_(false)
{}
template<class CloudType>
mousse::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
  const GradientDispersionRAS<CloudType>& dm
)
:
  DispersionRASModel<CloudType>(dm),
  gradkPtr_(dm.gradkPtr_),
  ownGradK_(dm.ownGradK_)
{
  dm.ownGradK_ = false;
}
// Destructor 
template<class CloudType>
mousse::GradientDispersionRAS<CloudType>::~GradientDispersionRAS()
{
  cacheFields(false);
}
// Member Functions 
template<class CloudType>
void mousse::GradientDispersionRAS<CloudType>::cacheFields(const bool store)
{
  DispersionRASModel<CloudType>::cacheFields(store);
  if (store)
  {
    gradkPtr_ = fvc::grad(*this->kPtr_).ptr();
    ownGradK_ = true;
  }
  else
  {
    if (ownGradK_)
    {
      deleteDemandDrivenData(gradkPtr_);
      gradkPtr_ = NULL;
      ownGradK_ = false;
    }
  }
}
template<class CloudType>
mousse::vector mousse::GradientDispersionRAS<CloudType>::update
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
  const vector& gradk = this->gradkPtr_->internalField()[cellI];
  const scalar UrelMag = mag(U - Uc - UTurb);
  const scalar tTurbLoc =
    min(k/epsilon, cps*pow(k, 1.5)/epsilon/(UrelMag + SMALL));
  // Parcel is perturbed by the turbulence
  if (dt < tTurbLoc)
  {
    tTurb += dt;
    if (tTurb > tTurbLoc)
    {
      tTurb = 0.0;
      const scalar sigma = sqrt(2.0*k/3.0);
      const vector dir = -gradk/(mag(gradk) + SMALL);
      scalar fac = 0.0;
      // In 2D calculations the -grad(k) is always
      // away from the axis of symmetry
      // This creates a 'hole' in the spray and to
      // prevent this we let fac be both negative/positive
      if (this->owner().mesh().nSolutionD() == 2)
      {
        fac = rnd.GaussNormal<scalar>();
      }
      else
      {
        fac = mag(rnd.GaussNormal<scalar>());
      }
      UTurb = sigma*fac*dir;
    }
  }
  else
  {
    tTurb = GREAT;
    UTurb = vector::zero;
  }
  return Uc + UTurb;
}

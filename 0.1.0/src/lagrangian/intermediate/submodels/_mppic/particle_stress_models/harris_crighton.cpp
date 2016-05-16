// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "harris_crighton.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace ParticleStressModels {

DEFINE_TYPE_NAME_AND_DEBUG(HarrisCrighton, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  ParticleStressModel,
  HarrisCrighton,
  dictionary
);

}
}


// Constructors
mousse::ParticleStressModels::HarrisCrighton::HarrisCrighton
(
  const dictionary& dict
)
:
  ParticleStressModel{dict},
  pSolid_{readScalar(dict.lookup("pSolid"))},
  beta_{readScalar(dict.lookup("beta"))},
  eps_{readScalar(dict.lookup("eps"))}
{}


mousse::ParticleStressModels::HarrisCrighton::HarrisCrighton
(
  const HarrisCrighton& hc
)
:
  ParticleStressModel{hc},
  pSolid_{hc.pSolid_},
  beta_{hc.beta_},
  eps_{hc.eps_}
{}


// Destructor
mousse::ParticleStressModels::HarrisCrighton::~HarrisCrighton()
{}


// Privare Member Functions
mousse::tmp<mousse::Field<mousse::scalar> >
mousse::ParticleStressModels::HarrisCrighton::denominator
(
  const Field<scalar>& alpha
) const
{
  return max(alphaPacked_ - alpha, max(eps_*(1.0 - alpha), SMALL));
}


// Member Functions
mousse::tmp<mousse::Field<mousse::scalar>>
mousse::ParticleStressModels::HarrisCrighton::tau
(
  const Field<scalar>& alpha,
  const Field<scalar>& /*rho*/,
  const Field<scalar>& /*uSqr*/
) const
{
  return pSolid_*pow(alpha, beta_)/denominator(alpha);
}


mousse::tmp<mousse::Field<mousse::scalar>>
mousse::ParticleStressModels::HarrisCrighton::dTaudTheta
(
  const Field<scalar>& alpha,
  const Field<scalar>& /*rho*/,
  const Field<scalar>& /*uSqr*/
) const
{
  const Field<scalar> d{denominator(alpha)};
  return pSolid_*pow(alpha, beta_)/d*(beta_/alpha + 1.0/d);
}


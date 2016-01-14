// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "exponential.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace ParticleStressModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(exponential, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    ParticleStressModel,
    exponential,
    dictionary
  );
}
}
// Constructors 
mousse::ParticleStressModels::exponential::exponential
(
  const dictionary& dict
)
:
  ParticleStressModel(dict),
  preExp_(readScalar(dict.lookup("preExp"))),
  expMax_(readScalar(dict.lookup("expMax"))),
  g0_(readScalar(dict.lookup("g0")))
{}
mousse::ParticleStressModels::exponential::exponential
(
  const exponential& hc
)
:
  ParticleStressModel(hc),
  preExp_(hc.preExp_),
  expMax_(hc.expMax_),
  g0_(hc.g0_)
{}
// Destructor 
mousse::ParticleStressModels::exponential::~exponential()
{}
// Member Functions 
mousse::tmp<mousse::Field<mousse::scalar> >
mousse::ParticleStressModels::exponential::tau
(
  const Field<scalar>& alpha,
  const Field<scalar>& rho,
  const Field<scalar>& uSqr
) const
{
  return dTaudTheta(alpha, rho, uSqr)/preExp_;
}
mousse::tmp<mousse::Field<mousse::scalar> >
mousse::ParticleStressModels::exponential::dTaudTheta
(
  const Field<scalar>& alpha,
  const Field<scalar>& /*rho*/,
  const Field<scalar>& /*uSqr*/
) const
{
  return
    g0_
   *min
    (
      exp(preExp_*(alpha - alphaPacked_)),
      expMax_
    );
}

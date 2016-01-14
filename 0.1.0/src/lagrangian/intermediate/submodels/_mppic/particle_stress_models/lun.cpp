// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lun.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace ParticleStressModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(Lun, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    ParticleStressModel,
    Lun,
    dictionary
  );
}
}
// Constructors
mousse::ParticleStressModels::Lun::Lun
(
  const dictionary& dict
)
:
  ParticleStressModel{dict},
  e_{readScalar(dict.lookup("e"))},
  eps_{readScalar(dict.lookup("eps"))}
{}
mousse::ParticleStressModels::Lun::Lun
(
  const Lun& ln
)
:
  ParticleStressModel{ln},
  e_{ln.e_},
  eps_{ln.eps_}
{}
// Destructor
mousse::ParticleStressModels::Lun::~Lun()
{}
// Member Functions
mousse::tmp<mousse::Field<mousse::scalar>>
mousse::ParticleStressModels::Lun::tau
(
  const Field<scalar>& alpha,
  const Field<scalar>& rho,
  const Field<scalar>& uSqr
) const
{
  tmp<Field<scalar>> g0
  (
    0.6
   / max
    (
      1.0 - pow(alpha/alphaPacked_,1.0/3.0),
      max(eps_*(1.0 - alpha), SMALL)
    )
  );
  tmp<Field<scalar>> gT(uSqr/3.0);
  return alpha*rho*(1.0 + alpha*(1.0 + e_)*g0)*gT;
}
mousse::tmp<mousse::Field<mousse::scalar>>
mousse::ParticleStressModels::Lun::dTaudTheta
(
  const Field<scalar>& /*alpha*/,
  const Field<scalar>& /*rho*/,
  const Field<scalar>& /*uSqr*/
) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::ParticleStressModels::Lun::dTau_dTheta"
    "(const Field<scalar>&, const Field<scalar>&, const Field<scalar>&) "
    "const"
  );
  return tmp<Field<scalar>>{NULL};
}

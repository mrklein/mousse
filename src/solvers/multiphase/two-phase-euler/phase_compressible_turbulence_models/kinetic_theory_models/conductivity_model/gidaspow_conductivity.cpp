// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gidaspow_conductivity.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace conductivityModels {
DEFINE_TYPE_NAME_AND_DEBUG(Gidaspow, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  conductivityModel,
  Gidaspow,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::conductivityModels::Gidaspow::Gidaspow
(
  const dictionary& dict
)
:
  conductivityModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::conductivityModels::Gidaspow::~Gidaspow()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::conductivityModels::Gidaspow::kappa
(
  const volScalarField& alpha1,
  const volScalarField& Theta,
  const volScalarField& g0,
  const volScalarField& rho1,
  const volScalarField& da,
  const dimensionedScalar& e
) const
{
  const scalar sqrtPi = sqrt(constant::mathematical::pi);
  return rho1*da*sqrt(Theta)*
  (
    2.0*sqr(alpha1)*g0*(1.0 + e)/sqrtPi
   + (9.0/8.0)*sqrtPi*g0*0.5*(1.0 + e)*sqr(alpha1)
   + (15.0/16.0)*sqrtPi*alpha1
   + (25.0/64.0)*sqrtPi/((1.0 + e)*g0)
  );
}

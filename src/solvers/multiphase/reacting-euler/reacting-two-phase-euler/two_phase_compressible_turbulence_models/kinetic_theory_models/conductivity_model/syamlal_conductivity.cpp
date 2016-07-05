// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "syamlal_conductivity.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace conductivityModels {
DEFINE_TYPE_NAME_AND_DEBUG(Syamlal, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  conductivityModel,
  Syamlal,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::conductivityModels::Syamlal::Syamlal
(
  const dictionary& dict
)
:
  conductivityModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::conductivityModels::Syamlal::~Syamlal()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::conductivityModels::Syamlal::kappa
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
    + (9.0/8.0)*sqrtPi*g0*0.25*sqr(1.0 + e)*(2.0*e - 1.0)*sqr(alpha1)
    /(49.0/16.0 - 33.0*e/16.0)
    + (15.0/32.0)*sqrtPi*alpha1/(49.0/16.0 - 33.0*e/16.0)
  );
}

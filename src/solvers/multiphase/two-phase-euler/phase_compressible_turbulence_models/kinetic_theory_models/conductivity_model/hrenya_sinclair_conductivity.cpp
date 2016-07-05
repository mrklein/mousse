// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hrenya_sinclair_conductivity.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace conductivityModels {
DEFINE_TYPE_NAME_AND_DEBUG(HrenyaSinclair, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  conductivityModel,
  HrenyaSinclair,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::conductivityModels::HrenyaSinclair::HrenyaSinclair
(
  const dictionary& dict
)
:
  conductivityModel{dict},
  coeffDict_{dict.subDict(typeName + "Coeffs")},
  L_{"L", dimensionSet(0, 1, 0, 0, 0), coeffDict_}
{}
// Destructor 
mousse::kineticTheoryModels::conductivityModels::HrenyaSinclair::
~HrenyaSinclair()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::conductivityModels::HrenyaSinclair::kappa
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
  volScalarField lamda
  {
    scalar(1) + da/(6.0*sqrt(2.0)*(alpha1 + scalar(1.0e-5)))/L_
  };
  return rho1*da*sqrt(Theta)*
  (
    2.0*sqr(alpha1)*g0*(1.0 + e)/sqrtPi
    + (9.0/8.0)*sqrtPi*g0*0.25*sqr(1.0 + e)*(2.0*e - 1.0)*sqr(alpha1)
    /(49.0/16.0 - 33.0*e/16.0)
    + (15.0/16.0)*sqrtPi*alpha1*(0.5*sqr(e) + 0.25*e - 0.75 + lamda)
    /((49.0/16.0 - 33.0*e/16.0)*lamda)
    + (25.0/64.0)*sqrtPi
    /((1.0 + e)*(49.0/16.0 - 33.0*e/16.0)*lamda*g0)
  );
}
bool mousse::kineticTheoryModels::conductivityModels::HrenyaSinclair::read()
{
  coeffDict_ <<= dict_.subDict(typeName + "Coeffs");
  L_.readIfPresent(coeffDict_);
  return true;
}

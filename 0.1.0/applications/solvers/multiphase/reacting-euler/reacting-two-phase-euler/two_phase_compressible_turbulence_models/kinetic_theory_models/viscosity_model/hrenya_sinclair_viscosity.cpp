// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hrenya_sinclair_viscosity.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace viscosityModels {
DEFINE_TYPE_NAME_AND_DEBUG(HrenyaSinclair, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  viscosityModel,
  HrenyaSinclair,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::viscosityModels::HrenyaSinclair::HrenyaSinclair
(
  const dictionary& dict
)
:
  viscosityModel{dict},
  coeffDict_{dict.subDict(typeName + "Coeffs")},
  L_{"L", dimLength, coeffDict_}
{}
// Destructor 
mousse::kineticTheoryModels::viscosityModels::HrenyaSinclair::~HrenyaSinclair()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::viscosityModels::HrenyaSinclair::nu
(
  const volScalarField& alpha1,
  const volScalarField& Theta,
  const volScalarField& g0,
  const volScalarField& /*rho1*/,
  const volScalarField& da,
  const dimensionedScalar& e
) const
{
  const scalar sqrtPi = sqrt(constant::mathematical::pi);
  volScalarField lamda
  {
    scalar(1) + da/(6.0*sqrt(2.0)*(alpha1 + scalar(1.0e-5)))/L_
  };
  return da*sqrt(Theta)*
  (
    (4.0/5.0)*sqr(alpha1)*g0*(1.0 + e)/sqrtPi
    + (1.0/15.0)*sqrtPi*g0*(1.0 + e)*(3.0*e - 1)*sqr(alpha1)/(3.0-e)
    + (1.0/6.0)*sqrtPi*alpha1*(0.5*lamda + 0.25*(3.0*e - 1.0))
    /(0.5*(3.0 - e)*lamda)
    + (10/96.0)*sqrtPi/((1.0 + e)*0.5*(3.0 - e)*g0*lamda)
  );
}
bool mousse::kineticTheoryModels::viscosityModels::HrenyaSinclair::read()
{
  coeffDict_ <<= dict_.subDict(typeName + "Coeffs");
  L_.readIfPresent(coeffDict_);
  return true;
}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "syamlal_viscosity.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace viscosityModels {
DEFINE_TYPE_NAME_AND_DEBUG(Syamlal, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(viscosityModel, Syamlal, dictionary);
}
}
}
// Constructors 
mousse::kineticTheoryModels::viscosityModels::Syamlal::Syamlal
(
  const dictionary& dict
)
:
  viscosityModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::viscosityModels::Syamlal::~Syamlal()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::viscosityModels::Syamlal::nu
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
  return da*sqrt(Theta)*
  (
    (4.0/5.0)*sqr(alpha1)*g0*(1.0 + e)/sqrtPi
    + (1.0/15.0)*sqrtPi*g0*(1.0 + e)*(3.0*e - 1.0)*sqr(alpha1)/(3.0 - e)
    + (1.0/6.0)*alpha1*sqrtPi/(3.0 - e)
  );
}

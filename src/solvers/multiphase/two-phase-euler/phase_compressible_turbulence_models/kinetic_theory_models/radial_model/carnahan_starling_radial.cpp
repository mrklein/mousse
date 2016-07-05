// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "carnahan_starling_radial.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace radialModels {
DEFINE_TYPE_NAME_AND_DEBUG(CarnahanStarling, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  radialModel,
  CarnahanStarling,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::radialModels::CarnahanStarling::CarnahanStarling
(
  const dictionary& dict
)
:
  radialModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::radialModels::CarnahanStarling::~CarnahanStarling()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::CarnahanStarling::g0
(
  const volScalarField& alpha,
  const dimensionedScalar& /*alphaMinFriction*/,
  const dimensionedScalar& /*alphaMax*/
) const
{
  return 1.0/(1.0 - alpha) + 3.0*alpha/(2.0*sqr(1.0 - alpha))
    + sqr(alpha)/(2.0*pow3(1.0 - alpha));
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::CarnahanStarling::g0prime
(
  const volScalarField& alpha,
  const dimensionedScalar& /*alphaMinFriction*/,
  const dimensionedScalar& /*alphaMax*/
) const
{
  return 2.5/sqr(1.0 - alpha) + 4.0*alpha/pow3(1.0 - alpha)
    + 1.5*sqr(alpha)/pow4(1.0 - alpha);
}

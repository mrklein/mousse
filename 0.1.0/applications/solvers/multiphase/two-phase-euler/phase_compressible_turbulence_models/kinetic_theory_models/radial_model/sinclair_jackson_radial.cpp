// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sinclair_jackson_radial.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace radialModels {
DEFINE_TYPE_NAME_AND_DEBUG(SinclairJackson, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  radialModel,
  SinclairJackson,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::radialModels::SinclairJackson::SinclairJackson
(
  const dictionary& dict
)
:
  radialModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::radialModels::SinclairJackson::~SinclairJackson()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::SinclairJackson::g0
(
  const volScalarField& alpha,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& alphaMax
) const
{
  return 1.0/(1.0 - cbrt(min(alpha, alphaMinFriction)/alphaMax));
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::SinclairJackson::g0prime
(
  const volScalarField& alpha,
  const dimensionedScalar& alphaMinFriction,
  const dimensionedScalar& alphaMax
) const
{
  volScalarField aByaMax
  (
    cbrt(min(max(alpha, scalar(1e-3)), alphaMinFriction)/alphaMax)
  );
  return (1.0/(3*alphaMax))/sqr(aByaMax - sqr(aByaMax));
}

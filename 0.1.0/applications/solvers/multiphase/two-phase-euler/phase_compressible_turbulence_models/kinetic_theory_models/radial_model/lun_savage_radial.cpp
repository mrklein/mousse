// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lun_savage_radial.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace kineticTheoryModels {
namespace radialModels {
DEFINE_TYPE_NAME_AND_DEBUG(LunSavage, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  radialModel,
  LunSavage,
  dictionary
);
}
}
}
// Constructors 
mousse::kineticTheoryModels::radialModels::LunSavage::LunSavage
(
  const dictionary& dict
)
:
  radialModel{dict}
{}
// Destructor 
mousse::kineticTheoryModels::radialModels::LunSavage::~LunSavage()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::LunSavage::g0
(
  const volScalarField& alpha,
  const dimensionedScalar& /*alphaMinFriction*/,
  const dimensionedScalar& alphaMax
) const
{
  return pow(1.0 - alpha/alphaMax, -2.5*alphaMax);
}
mousse::tmp<mousse::volScalarField>
mousse::kineticTheoryModels::radialModels::LunSavage::g0prime
(
  const volScalarField& alpha,
  const dimensionedScalar& /*alphaMinFriction*/,
  const dimensionedScalar& alphaMax
) const
{
  return 2.5*pow(1.0 - alpha/alphaMax, -2.5*alphaMax - 1);
}

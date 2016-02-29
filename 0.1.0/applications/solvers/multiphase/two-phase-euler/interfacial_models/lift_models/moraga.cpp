// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "moraga.hpp"
#include "phase_pair.hpp"
#include "fvc_grad.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace liftModels {
DEFINE_TYPE_NAME_AND_DEBUG(Moraga, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liftModel, Moraga, dictionary);
}
}
// Constructors 
mousse::liftModels::Moraga::Moraga
(
  const dictionary& dict,
  const phasePair& pair
)
:
  liftModel{dict, pair}
{}
// Destructor 
mousse::liftModels::Moraga::~Moraga()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::liftModels::Moraga::Cl() const
{
  volScalarField Re{pair_.Re()};
  volScalarField sqrSr
  {
    sqr(pair_.dispersed().d())/pair_.continuous().nu()
    *mag(fvc::grad(pair_.continuous().U()))
  };
  if (min(Re).value() < 1200.0
      || max(Re).value() > 18800.0
      || min(sqrSr).value() < 0.0016
      || max(sqrSr).value() > 0.04)
  {
    WARNING_IN
    (
      "mousse::tmp<mousse::volScalarField> "
      "mousse::liftModels::Moraga::Cl() const"
    )
    << "Re and/or Sr are out of the range of applicability of the "
    << "Moraga model. Clamping to range bounds"
    << endl;
  }
  Re.min(1200.0);
  Re.max(18800.0);
  sqrSr.min(0.0016);
  sqrSr.max(0.04);
  return 0.2*exp(- Re*sqrSr/3.6e5 - 0.12)*exp(Re*sqrSr/3.0e7);
}

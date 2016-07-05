// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "frossling.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace massTransferModels {
DEFINE_TYPE_NAME_AND_DEBUG(Frossling, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(massTransferModel, Frossling, dictionary);
}
}
// Constructors 
mousse::massTransferModels::Frossling::Frossling
(
  const dictionary& dict,
  const phasePair& pair
)
:
  massTransferModel{dict, pair},
  Le_{"Le", dimless, dict}
{}
// Destructor 
mousse::massTransferModels::Frossling::~Frossling()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::massTransferModels::Frossling::K() const
{
  volScalarField Sh{scalar(2) + 0.552*sqrt(pair_.Re())*cbrt(Le_*pair_.Pr())};
  return 6.0*pair_.dispersed()*Sh/sqr(pair_.dispersed().d());
}

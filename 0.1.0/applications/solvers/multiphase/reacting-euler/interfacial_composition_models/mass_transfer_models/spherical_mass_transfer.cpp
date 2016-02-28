// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_mass_transfer.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace massTransferModels {
DEFINE_TYPE_NAME_AND_DEBUG(sphericalMassTransfer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  massTransferModel,
  sphericalMassTransfer,
  dictionary
);
}
}
// Constructors 
mousse::massTransferModels::sphericalMassTransfer::sphericalMassTransfer
(
  const dictionary& dict,
  const phasePair& pair
)
:
  massTransferModel{dict, pair},
  Le_{"Le", dimless, dict}
{}
// Destructor 
mousse::massTransferModels::sphericalMassTransfer::~sphericalMassTransfer()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::massTransferModels::sphericalMassTransfer::K() const
{
  return 60.0*pair_.dispersed()/sqr(pair_.dispersed().d());
}

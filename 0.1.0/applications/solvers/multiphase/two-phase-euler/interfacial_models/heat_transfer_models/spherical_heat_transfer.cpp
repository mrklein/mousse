// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_heat_transfer.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace heatTransferModels {
DEFINE_TYPE_NAME_AND_DEBUG(sphericalHeatTransfer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  heatTransferModel,
  sphericalHeatTransfer,
  dictionary
);
}
}
// Constructors 
mousse::heatTransferModels::sphericalHeatTransfer::sphericalHeatTransfer
(
  const dictionary& dict,
  const phasePair& pair
)
:
  heatTransferModel{dict, pair}
{}
// Destructor 
mousse::heatTransferModels::sphericalHeatTransfer::~sphericalHeatTransfer()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::heatTransferModels::sphericalHeatTransfer::K() const
{
  return
    60.0*max(pair_.dispersed(), residualAlpha_)*pair_.continuous().kappa()
    /sqr(pair_.dispersed().d());
}

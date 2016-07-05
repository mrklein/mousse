// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(heatTransferModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(heatTransferModel, dictionary);
}
// Constructors 
mousse::heatTransferModel::heatTransferModel
(
  const dictionary& interfaceDict,
  const volScalarField& alpha1,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  interfaceDict_{interfaceDict},
  alpha1_{alpha1},
  phase1_{phase1},
  phase2_{phase2}
{}
// Destructor 
mousse::heatTransferModel::~heatTransferModel()
{}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(heatTransferModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(heatTransferModel, dictionary);
}
const mousse::dimensionSet mousse::heatTransferModel::dimK{1, -1, -3, -1, 0};
// Constructors 
mousse::heatTransferModel::heatTransferModel
(
  const dictionary& dict,
  const phasePair& pair
)
:
  pair_{pair},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    dict.lookupOrDefault<scalar>
    (
      "residualAlpha",
      pair_.dispersed().residualAlpha().value()
    )
  }
{}
// Destructor 
mousse::heatTransferModel::~heatTransferModel()
{}

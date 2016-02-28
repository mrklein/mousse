// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mass_transfer_model.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(massTransferModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(massTransferModel, dictionary);
}
const mousse::dimensionSet mousse::massTransferModel::dimK{0, -2, 0, 0, 0};
// Constructors 
mousse::massTransferModel::massTransferModel
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  pair_{pair}
{}
// Destructor 
mousse::massTransferModel::~massTransferModel()
{}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ranz_marshall.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace heatTransferModels {
DEFINE_TYPE_NAME_AND_DEBUG(RanzMarshall, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(heatTransferModel, RanzMarshall, dictionary);
}
}
// Constructors 
mousse::heatTransferModels::RanzMarshall::RanzMarshall
(
  const dictionary& dict,
  const phasePair& pair
)
:
  heatTransferModel{dict, pair}
{}
// Destructor 
mousse::heatTransferModels::RanzMarshall::~RanzMarshall()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::heatTransferModels::RanzMarshall::K(const scalar residualAlpha) const
{
  volScalarField Nu{scalar(2) + 0.6*sqrt(pair_.Re())*cbrt(pair_.Pr())};
  return 6.0*max(pair_.dispersed(), residualAlpha)*pair_.continuous().kappa()
    *Nu/sqr(pair_.dispersed().d());
}

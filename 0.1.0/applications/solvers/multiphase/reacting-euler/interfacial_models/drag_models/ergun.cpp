// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ergun.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(Ergun, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, Ergun, dictionary);
}
}
// Constructors 
mousse::dragModels::Ergun::Ergun
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject}
{}
// Destructor 
mousse::dragModels::Ergun::~Ergun()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::Ergun::CdRe() const
{
  return
    (4/3)*(150*max(scalar(1) - pair_.continuous(),
                   pair_.continuous().residualAlpha())
           /max(pair_.continuous(), pair_.continuous().residualAlpha())
           + 1.75*pair_.Re());
}

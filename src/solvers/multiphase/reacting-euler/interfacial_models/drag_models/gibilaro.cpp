// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gibilaro.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(Gibilaro, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, Gibilaro, dictionary);
}
}
// Constructors 
mousse::dragModels::Gibilaro::Gibilaro
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject}
{}
// Destructor 
mousse::dragModels::Gibilaro::~Gibilaro()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::Gibilaro::CdRe() const
{
  volScalarField alpha2
  {
    max(scalar(1) - pair_.dispersed(), pair_.continuous().residualAlpha())
  };
  return
    (4/3)*(17.3/alpha2 + 0.336*pair_.Re())
    *max(pair_.continuous(), pair_.continuous().residualAlpha())
    *pow(alpha2, -2.8);
}

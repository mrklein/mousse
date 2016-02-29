// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wellek.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace aspectRatioModels {
DEFINE_TYPE_NAME_AND_DEBUG(Wellek, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  aspectRatioModel,
  Wellek,
  dictionary
);
}
}
// Constructors 
mousse::aspectRatioModels::Wellek::Wellek
(
  const dictionary& dict,
  const phasePair& pair
)
:
  aspectRatioModel{dict, pair}
{}
// Destructor 
mousse::aspectRatioModels::Wellek::~Wellek()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::aspectRatioModels::Wellek::E() const
{
  return scalar(1)/(scalar(1) + 0.163*pow(pair_.Eo(), 0.757));
}

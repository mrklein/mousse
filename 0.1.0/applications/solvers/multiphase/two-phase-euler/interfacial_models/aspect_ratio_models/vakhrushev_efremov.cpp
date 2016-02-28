// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vakhrushev_efremov.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace aspectRatioModels {
DEFINE_TYPE_NAME_AND_DEBUG(VakhrushevEfremov, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  aspectRatioModel,
  VakhrushevEfremov,
  dictionary
);
}
}
// Constructors 
mousse::aspectRatioModels::VakhrushevEfremov::VakhrushevEfremov
(
  const dictionary& dict,
  const phasePair& pair
)
:
  aspectRatioModel{dict, pair}
{}
// Destructor 
mousse::aspectRatioModels::VakhrushevEfremov::~VakhrushevEfremov()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::aspectRatioModels::VakhrushevEfremov::E() const
{
  volScalarField Ta{pair_.Ta()};
  return neg(Ta - scalar(1))*scalar(1)
    + pos(Ta - scalar(1))*neg(Ta - scalar(39.8))
    *pow3(0.81 + 0.206*tanh(1.6 - 2*log10(max(Ta, scalar(1)))))
    + pos(Ta - scalar(39.8))*0.24;
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blended.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(blended, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  blended,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::blended::blended
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2},
  dragModel1_{New(interfaceDict.subDict(phase1.name()), phase1, phase2)},
  dragModel2_{New(interfaceDict.subDict(phase2.name()), phase2, phase1)}
{}
// Destructor 
mousse::dragModels::blended::~blended()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::blended::K
(
  const volScalarField& Ur
) const
{
  return phase2()*dragModel1_->K(Ur) + phase1()*dragModel2_->K(Ur);
}

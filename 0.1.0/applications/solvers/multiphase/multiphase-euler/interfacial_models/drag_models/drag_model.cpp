// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "drag_model.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(dragModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(dragModel, dictionary);
}
// Constructors 
mousse::dragModel::dragModel
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  interfaceDict_{interfaceDict},
  phase1_{phase1},
  phase2_{phase2},
  residualPhaseFraction_
  {
    "residualPhaseFraction",
    dimless,
    interfaceDict.lookup("residualPhaseFraction")
  },
  residualSlip_
  {
    "residualSlip",
    dimVelocity,
    interfaceDict.lookup("residualSlip")
  }
{}
// Destructor 
mousse::dragModel::~dragModel()
{}

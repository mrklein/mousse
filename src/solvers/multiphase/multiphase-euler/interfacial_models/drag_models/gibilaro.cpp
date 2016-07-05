// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gibilaro.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(Gibilaro, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  Gibilaro,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::Gibilaro::Gibilaro
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::Gibilaro::~Gibilaro()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::Gibilaro::K
(
  const volScalarField& Ur
) const
{
  volScalarField alpha2{max(phase2_, scalar(1.0e-6))};
  volScalarField bp{pow(alpha2, -2.8)};
  volScalarField Re{max(alpha2*Ur*phase1_.d()/phase2_.nu(), scalar(1.0e-3))};
  return (17.3/Re + scalar(0.336))*phase2_.rho()*Ur*bp/phase1_.d();
}

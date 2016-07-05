// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ergun.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(Ergun, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  Ergun,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::Ergun::Ergun
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::Ergun::~Ergun()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::Ergun::K
(
  const volScalarField& Ur
) const
{
  volScalarField alpha2{max(phase2_, scalar{1.0e-6})};
  return
    150.0*phase1_*phase2_.nu()*phase2_.rho()
    /sqr(alpha2*phase1_.d())
    + 1.75*phase2_.rho()*Ur/(alpha2*phase1_.d());
}

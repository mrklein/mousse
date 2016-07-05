// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wen_yu.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(WenYu, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  WenYu,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::WenYu::WenYu
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::WenYu::~WenYu()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::WenYu::K
(
  const volScalarField& Ur
) const
{
  volScalarField alpha2{max(phase2_, scalar(1.0e-6))};
  volScalarField bp{pow(alpha2, -2.65)};
  volScalarField Re{max(Ur*phase1_.d()/phase2_.nu(), scalar(1.0e-3))};
  volScalarField Cds
  {
    neg(Re - 1000)*(24.0*(1.0 + 0.15*pow(Re, 0.687))/Re) + pos(Re - 1000)*0.44
  };
  return 0.75*Cds*phase2_.rho()*Ur*bp/phase1_.d();
}

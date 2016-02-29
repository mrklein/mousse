// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gidaspow_ergun_wen_yu.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(GidaspowErgunWenYu, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  GidaspowErgunWenYu,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::GidaspowErgunWenYu::GidaspowErgunWenYu
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::GidaspowErgunWenYu::~GidaspowErgunWenYu()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::GidaspowErgunWenYu::K
(
  const volScalarField& Ur
) const
{
  volScalarField alpha2{max(phase2_, scalar(1.0e-6))};
  volScalarField d{phase1_.d()};
  volScalarField bp{pow(alpha2, -2.65)};
  volScalarField Re{max(Ur*d/phase2_.nu(), scalar(1.0e-3))};
  volScalarField Cds
  {
    neg(Re - 1000)*(24.0*(1.0 + 0.15*pow(Re, 0.687))/Re)
    + pos(Re - 1000)*0.44
  };
  // Wen and Yu (1966)
  return
  (
    pos(alpha2 - 0.8)
    *(0.75*Cds*phase2_.rho()*Ur*bp/d)
    + neg(alpha2 - 0.8)
    *(150.0*phase1_*phase2_.nu()*phase2_.rho()/(sqr(alpha2*d))
      + 1.75*phase2_.rho()*Ur/(alpha2*d)));
}

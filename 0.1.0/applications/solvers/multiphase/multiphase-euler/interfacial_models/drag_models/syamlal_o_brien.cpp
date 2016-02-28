// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "syamlal_o_brien.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(SyamlalOBrien, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  SyamlalOBrien,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::SyamlalOBrien::SyamlalOBrien
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::SyamlalOBrien::~SyamlalOBrien()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::SyamlalOBrien::K
(
  const volScalarField& Ur
) const
{
  volScalarField alpha2{max(phase2_, scalar(1.0e-6))};
  volScalarField A{pow(alpha2, 4.14)};
  volScalarField B
  {
    neg(alpha2 - 0.85)*(0.8*pow(alpha2, 1.28))
    + pos(alpha2 - 0.85)*(pow(alpha2, 2.65))
  };
  volScalarField Re{max(Ur*phase1_.d()/phase2_.nu(), scalar(1.0e-3))};
  volScalarField Vr
  {
    0.5*(A - 0.06*Re + sqrt(sqr(0.06*Re) + 0.12*Re*(2.0*B - A) + sqr(A)))
  };
  volScalarField Cds{sqr(0.63 + 4.8*sqrt(Vr/Re))};
  return 0.75*Cds*phase2_.rho()*Ur/(phase1_.d()*sqr(Vr));
}

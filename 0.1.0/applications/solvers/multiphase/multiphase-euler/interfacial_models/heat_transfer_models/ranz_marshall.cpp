// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ranz_marshall.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace heatTransferModels {
DEFINE_TYPE_NAME_AND_DEBUG(RanzMarshall, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  heatTransferModel,
  RanzMarshall,
  dictionary
);
}
}
// Constructors 
mousse::heatTransferModels::RanzMarshall::RanzMarshall
(
  const dictionary& interfaceDict,
  const volScalarField& alpha1,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  heatTransferModel{interfaceDict, alpha1, phase1, phase2}
{}
// Destructor 
mousse::heatTransferModels::RanzMarshall::~RanzMarshall()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::heatTransferModels::RanzMarshall::K
(
  const volScalarField& Ur
) const
{
  volScalarField Re{max(Ur*phase1_.d()/phase2_.nu(), scalar(1.0e-3))};
  dimensionedScalar Prb = phase2_.rho()*phase2_.nu()*phase2_.Cp()/phase2_.kappa();
  volScalarField Nu{scalar(2) + 0.6*sqrt(Re)*cbrt(Prb)};
  return 6.0*phase2_.kappa()*Nu/sqr(phase1_.d());
}

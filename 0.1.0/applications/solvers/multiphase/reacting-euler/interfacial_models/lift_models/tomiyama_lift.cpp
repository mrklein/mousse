// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_lift.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace liftModels {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaLift, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liftModel, TomiyamaLift, dictionary);
}
}
// Constructors 
mousse::liftModels::TomiyamaLift::TomiyamaLift
(
  const dictionary& dict,
  const phasePair& pair
)
:
  liftModel{dict, pair}
{}
// Destructor 
mousse::liftModels::TomiyamaLift::~TomiyamaLift()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::liftModels::TomiyamaLift::Cl() const
{
  volScalarField EoH{pair_.EoH2()};
  volScalarField f
  {
    0.0010422*pow3(EoH) - 0.0159*sqr(EoH) - 0.0204*EoH + 0.474
  };
  return neg(EoH - scalar(4))*min(0.288*tanh(0.121*pair_.Re()), f)
    + pos(EoH - scalar(4))*neg(EoH - scalar(10.7))*f
    + pos(EoH - scalar(10.7))*(-0.288);
}

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "instability_xi_eq.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace XiEqModels {

DEFINE_TYPE_NAME_AND_DEBUG(instability, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiEqModel, instability, dictionary);

}
}


// Constructors 
mousse::XiEqModels::instability::instability
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModel{XiEqProperties, thermo, turbulence, Su},
  XiEqIn{readScalar(XiEqModelCoeffs_.lookup("XiEqIn"))},
  XiEqModel_{XiEqModel::New(XiEqModelCoeffs_, thermo, turbulence, Su)}
{}


// Destructor 
mousse::XiEqModels::instability::~instability()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiEqModels::instability::XiEq() const
{
  volScalarField turbXiEq{XiEqModel_->XiEq()};
  return XiEqIn/turbXiEq + turbXiEq;
}


bool mousse::XiEqModels::instability::read(const dictionary& XiEqProperties)
{
  XiEqModel::read(XiEqProperties);
  XiEqModelCoeffs_.lookup("XiEqIn") >> XiEqIn;
  return XiEqModel_->read(XiEqModelCoeffs_);
}


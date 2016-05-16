// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "algebraic.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace XiModels {

DEFINE_TYPE_NAME_AND_DEBUG(algebraic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiModel, algebraic, dictionary);

}
}


// Constructors 
mousse::XiModels::algebraic::algebraic
(
  const dictionary& XiProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su,
  const volScalarField& rho,
  const volScalarField& b,
  const surfaceScalarField& phi
)
:
  XiModel{XiProperties, thermo, turbulence, Su, rho, b, phi},
  XiShapeCoef{readScalar(XiModelCoeffs_.lookup("XiShapeCoef"))},
  XiEqModel_{XiEqModel::New(XiProperties, thermo, turbulence, Su)},
  XiGModel_{XiGModel::New(XiProperties, thermo, turbulence, Su)}
{}


// Destructor 
mousse::XiModels::algebraic::~algebraic()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiModels::algebraic::Db() const
{
  return XiGModel_->Db();
}


void mousse::XiModels::algebraic::correct()
{
  volScalarField XiEqEta{XiEqModel_->XiEq()};
  volScalarField GEta{XiGModel_->G()};
  volScalarField R{GEta*XiEqEta/(XiEqEta - 0.999)};
  volScalarField XiEqStar{R/(R - GEta)};
  Xi_ == 1.0 + (1.0 + (2*XiShapeCoef)*(0.5 - b_))*(XiEqStar - 1.0);
}


bool mousse::XiModels::algebraic::read(const dictionary& XiProperties)
{
  XiModel::read(XiProperties);
  XiModelCoeffs_.lookup("XiShapeCoef") >> XiShapeCoef;
  return true;
}


// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "instability_g.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace XiGModels {

DEFINE_TYPE_NAME_AND_DEBUG(instabilityG, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiGModel, instabilityG, dictionary);

}
}


// Constructors 
mousse::XiGModels::instabilityG::instabilityG
(
  const dictionary& XiGProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiGModel{XiGProperties, thermo, turbulence, Su},
  GIn_{XiGModelCoeffs_.lookup("GIn")},
  lambdaIn_{XiGModelCoeffs_.lookup("lambdaIn")},
  XiGModel_{XiGModel::New(XiGModelCoeffs_, thermo, turbulence, Su)}
{}


// Destructor 
mousse::XiGModels::instabilityG::~instabilityG()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiGModels::instabilityG::G() const
{
  volScalarField turbXiG{XiGModel_->G()};
  return (GIn_*GIn_/(GIn_ + turbXiG) + turbXiG);
}


mousse::tmp<mousse::volScalarField> mousse::XiGModels::instabilityG::Db() const
{
  const objectRegistry& db = Su_.db();
  const volScalarField& Xi = db.lookupObject<volScalarField>("Xi");
  const volScalarField& rho = db.lookupObject<volScalarField>("rho");
  const volScalarField& mgb = db.lookupObject<volScalarField>("mgb");
  return XiGModel_->Db()
    + rho*Su_*(Xi - 1.0)*mgb*(0.5*lambdaIn_)/(mgb + 1.0/lambdaIn_);
}


bool mousse::XiGModels::instabilityG::read(const dictionary& XiGProperties)
{
  XiGModel::read(XiGProperties);
  XiGModelCoeffs_.lookup("GIn") >> GIn_;
  XiGModelCoeffs_.lookup("lambdaIn") >> lambdaIn_;
  return true;
}


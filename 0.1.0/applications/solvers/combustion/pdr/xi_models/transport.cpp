// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transport.hpp"
#include "surface_interpolate.hpp"
#include "fvm_ddt.hpp"
#include "fvc_laplacian.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiModels
{
DEFINE_TYPE_NAME_AND_DEBUG(transport, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiModel, transport, dictionary);
}
}
// Constructors 
mousse::XiModels::transport::transport
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
mousse::XiModels::transport::~transport()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiModels::transport::Db() const
{
  return XiGModel_->Db();
}
void mousse::XiModels::transport::correct
(
  const fv::convectionScheme<scalar>& mvConvection
)
{
  volScalarField XiEqEta{XiEqModel_->XiEq()};
  volScalarField GEta{XiGModel_->G()};
  volScalarField R{GEta*XiEqEta/(XiEqEta - 0.999)};
  volScalarField XiEqStar{R/(R - GEta)};
  volScalarField XiEq{1.0 + (1.0 + (2*XiShapeCoef)*(0.5 - b_))*(XiEqStar - 1.0)};
  volScalarField G{R*(XiEq - 1.0)/XiEq};
  const objectRegistry& db = b_.db();
  const volScalarField& betav = db.lookupObject<volScalarField>("betav");
  const volScalarField& mgb = db.lookupObject<volScalarField>("mgb");
  const surfaceScalarField& phiSt =
    db.lookupObject<surfaceScalarField>("phiSt");
  const volScalarField& Db = db.lookupObject<volScalarField>("Db");
  const surfaceScalarField& nf = db.lookupObject<surfaceScalarField>("nf");
  surfaceScalarField phiXi
  {
    "phiXi",
    phiSt + (-fvc::interpolate(fvc::laplacian(Db, b_)/mgb)*nf
             + fvc::interpolate(rho_)*fvc::interpolate(Su_*(1.0/Xi_ - Xi_))*nf)
  };
  solve
  (
    betav*fvm::ddt(rho_, Xi_)
    + mvConvection.fvmDiv(phi_, Xi_)
    + fvm::div(phiXi, Xi_)
    - fvm::Sp(fvc::div(phiXi), Xi_)
    ==
    betav*rho_*R
    - fvm::Sp(betav*rho_*(R - G), Xi_)
  );
  // Correct boundedness of Xi
  // ~~~~~~~~~~~~~~~~~~~~~~~~~
  Xi_.max(1.0);
  Xi_ = min(Xi_, 2.0*XiEq);
}
bool mousse::XiModels::transport::read(const dictionary& XiProperties)
{
  XiModel::read(XiProperties);
  XiModelCoeffs_.lookup("XiShapeCoef") >> XiShapeCoef;
  return true;
}

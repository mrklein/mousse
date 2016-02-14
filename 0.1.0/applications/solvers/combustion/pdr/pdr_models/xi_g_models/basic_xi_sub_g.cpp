// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_xi_sub_g.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiGModels
{
DEFINE_TYPE_NAME_AND_DEBUG(basicSubGrid, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiGModel, basicSubGrid, dictionary);
};
};
// Constructors 
mousse::XiGModels::basicSubGrid::basicSubGrid
(
  const dictionary& XiGProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiGModel{XiGProperties, thermo, turbulence, Su},
  k1{readScalar(XiGModelCoeffs_.lookup("k1"))},
  XiGModel_{XiGModel::New(XiGModelCoeffs_, thermo, turbulence, Su)}
{}
// Destructor 
mousse::XiGModels::basicSubGrid::~basicSubGrid()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiGModels::basicSubGrid::G() const
{
  const objectRegistry& db = Su_.db();
  const volVectorField& U = db.lookupObject<volVectorField>("U");
  const volScalarField& Nv = db.lookupObject<volScalarField>("Nv");
  const volScalarField& Lobs = db.lookupObject<volScalarField>("Lobs");
  tmp<volScalarField> tGtot = XiGModel_->G();
  volScalarField& Gtot = tGtot();
  const scalarField Cw = pow(Su_.mesh().V(), 2.0/3.0);
  tmp<volScalarField> tN
  {
    new volScalarField
    {
      {
        "tN",
        Su_.mesh().time().timeName(),
        Su_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      Su_.mesh(),
      {"zero", Nv.dimensions(), 0.0},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  volScalarField& N = tN();
  N.internalField() = Nv.internalField()*Cw;
  FOR_ALL(N, celli)
  {
    if (N[celli] > 1e-3)
    {
      Gtot[celli] += k1*mag(U[celli])/Lobs[celli];
    }
  }
  return tGtot;
}
mousse::tmp<mousse::volScalarField> mousse::XiGModels::basicSubGrid::Db() const
{
  const objectRegistry& db = Su_.db();
  const volScalarField& Xi = db.lookupObject<volScalarField>("Xi");
  const volScalarField& rho = db.lookupObject<volScalarField>("rho");
  const volScalarField& mgb = db.lookupObject<volScalarField>("mgb");
  const volScalarField& Lobs = db.lookupObject<volScalarField>("Lobs");
  return XiGModel_->Db()
    + rho*Su_*(Xi - 1.0)*mgb*(0.5*Lobs)*Lobs/(mgb*Lobs + 1.0);
}
bool mousse::XiGModels::basicSubGrid::read(const dictionary& XiGProperties)
{
  XiGModel::read(XiGProperties);
  XiGModelCoeffs_.lookup("k1") >> k1;
  return true;
}

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "standard_radiation.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(standardRadiation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmRadiationModel,
  standardRadiation,
  dictionary
);
// Constructors 
standardRadiation::standardRadiation
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmRadiationModel(typeName, owner, dict),
  QinPrimary_
  (
    IOobject
    (
      "Qin", // same name as Qin on primary region to enable mapping
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    owner.regionMesh(),
    dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0),
    owner.mappedPushedFieldPatchTypes<scalar>()
  ),
  QrNet_
  (
    IOobject
    (
      "QrNet",
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    owner.regionMesh(),
    dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0),
    zeroGradientFvPatchScalarField::typeName
  ),
  beta_(readScalar(coeffDict_.lookup("beta"))),
  kappaBar_(readScalar(coeffDict_.lookup("kappaBar")))
{}
// Destructor 
standardRadiation::~standardRadiation()
{}
// Member Functions 
void standardRadiation::correct()
{
  // Transfer Qr from primary region
  QinPrimary_.correctBoundaryConditions();
}
tmp<volScalarField> standardRadiation::Shs()
{
  tmp<volScalarField> tShs
  (
    new volScalarField
    (
      IOobject
      (
        typeName + ":Shs",
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      owner().regionMesh(),
      dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  scalarField& Shs = tShs();
  const scalarField& QinP = QinPrimary_.internalField();
  const scalarField& delta = owner_.delta().internalField();
  const scalarField& alpha = owner_.alpha().internalField();
  Shs = beta_*QinP*alpha*(1.0 - exp(-kappaBar_*delta));
  // Update net Qr on local region
  QrNet_.internalField() = QinP - Shs;
  QrNet_.correctBoundaryConditions();
  return tShs;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

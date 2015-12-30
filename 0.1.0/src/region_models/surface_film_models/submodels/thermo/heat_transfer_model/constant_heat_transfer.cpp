// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_heat_transfer.hpp"
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
defineTypeNameAndDebug(constantHeatTransfer, 0);
addToRunTimeSelectionTable
(
  heatTransferModel,
  constantHeatTransfer,
  dictionary
);
// Constructors 
constantHeatTransfer::constantHeatTransfer
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  heatTransferModel(typeName, owner, dict),
  c0_(readScalar(coeffDict_.lookup("c0")))
{}
// Destructor 
constantHeatTransfer::~constantHeatTransfer()
{}
// Member Functions 
void constantHeatTransfer::correct()
{
  // do nothing
}
tmp<volScalarField> constantHeatTransfer::h() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "htc",
        owner_.time().timeName(),
        owner_.regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      owner_.regionMesh(),
      dimensionedScalar
      (
        "c0",
        dimEnergy/dimTime/sqr(dimLength)/dimTemperature,
        c0_
      ),
      zeroGradientFvPatchScalarField::typeName
    )
  );
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

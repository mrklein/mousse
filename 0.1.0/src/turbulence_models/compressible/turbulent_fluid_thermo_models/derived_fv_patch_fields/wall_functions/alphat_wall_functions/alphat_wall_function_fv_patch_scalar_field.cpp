// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alphat_wall_function_fv_patch_scalar_field.hpp"
#include "compressible_turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace compressible
{
// Constructors 
alphatWallFunctionFvPatchScalarField::alphatWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  Prt_(0.85)
{}
alphatWallFunctionFvPatchScalarField::alphatWallFunctionFvPatchScalarField
(
  const alphatWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  Prt_(ptf.Prt_)
{}
alphatWallFunctionFvPatchScalarField::alphatWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF, dict),
  Prt_(dict.lookupOrDefault<scalar>("Prt", 0.85))
{}
alphatWallFunctionFvPatchScalarField::alphatWallFunctionFvPatchScalarField
(
  const alphatWallFunctionFvPatchScalarField& awfpsf
)
:
  fixedValueFvPatchScalarField(awfpsf),
  Prt_(awfpsf.Prt_)
{}
alphatWallFunctionFvPatchScalarField::alphatWallFunctionFvPatchScalarField
(
  const alphatWallFunctionFvPatchScalarField& awfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(awfpsf, iF),
  Prt_(awfpsf.Prt_)
{}
// Member Functions 
void alphatWallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const label patchi = patch().index();
  // Retrieve turbulence properties from model
  const compressibleTurbulenceModel& turbModel =
    db().lookupObject<compressibleTurbulenceModel>
    (
      IOobject::groupName
      (
        compressibleTurbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
  const tmp<scalarField> tnutw = turbModel.nut(patchi);
  operator==(rhow*tnutw/Prt_);
  fixedValueFvPatchScalarField::updateCoeffs();
}
void alphatWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("Prt") << Prt_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
makePatchTypeField
(
  fvPatchScalarField,
  alphatWallFunctionFvPatchScalarField
);
}  // namespace compressible
}  // namespace mousse

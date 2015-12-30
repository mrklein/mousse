// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_shear_stress_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "turbulence_model.hpp"
// Constructors 
mousse::fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(p, iF),
  tau0_(vector::zero)
{}
mousse::fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField(p, iF),
  tau0_(dict.lookupOrDefault<vector>("tau", vector::zero))
{
  fvPatchField<vector>::operator=(patchInternalField());
}
mousse::fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
  const fixedShearStressFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField(ptf, p, iF, mapper),
  tau0_(ptf.tau0_)
{}
mousse::fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
  const fixedShearStressFvPatchVectorField& ptf
)
:
  fixedValueFvPatchVectorField(ptf),
  tau0_(ptf.tau0_)
{}
mousse::fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
  const fixedShearStressFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(ptf, iF),
  tau0_(ptf.tau0_)
{}
// Member Functions 
void mousse::fixedShearStressFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  scalarField nuEff(turbModel.nuEff()()[patch().index()]);
  const vectorField Uc(patchInternalField());
  vector tauHat = tau0_/(mag(tau0_) + ROOTVSMALL);
  const scalarField& ry = patch().deltaCoeffs();
  operator==(tauHat*(tauHat & (tau0_*(1.0/(ry*nuEff)) + Uc)));
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::fixedShearStressFvPatchVectorField::write(Ostream& os) const
{
  fixedValueFvPatchVectorField::write(os);
  os.writeKeyword("tau") << tau0_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchVectorField,
    fixedShearStressFvPatchVectorField
  );
}

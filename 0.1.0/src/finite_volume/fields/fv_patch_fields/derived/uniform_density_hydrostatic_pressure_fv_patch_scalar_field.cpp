// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_density_hydrostatic_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
// Constructors 
mousse::uniformDensityHydrostaticPressureFvPatchScalarField::
uniformDensityHydrostaticPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  rho_(0.0),
  pRefValue_(0.0),
  pRefPoint_(vector::zero)
{}
mousse::uniformDensityHydrostaticPressureFvPatchScalarField::
uniformDensityHydrostaticPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF),
  rho_(readScalar(dict.lookup("rho"))),
  pRefValue_(readScalar(dict.lookup("pRefValue"))),
  pRefPoint_(dict.lookup("pRefPoint"))
{
  if (dict.found("value"))
  {
    fvPatchField<scalar>::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    evaluate();
  }
}
mousse::uniformDensityHydrostaticPressureFvPatchScalarField::
uniformDensityHydrostaticPressureFvPatchScalarField
(
  const uniformDensityHydrostaticPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  rho_(ptf.rho_),
  pRefValue_(ptf.pRefValue_),
  pRefPoint_(ptf.pRefPoint_)
{}
mousse::uniformDensityHydrostaticPressureFvPatchScalarField::
uniformDensityHydrostaticPressureFvPatchScalarField
(
  const uniformDensityHydrostaticPressureFvPatchScalarField& ptf
)
:
  fixedValueFvPatchScalarField(ptf),
  rho_(ptf.rho_),
  pRefValue_(ptf.pRefValue_),
  pRefPoint_(ptf.pRefPoint_)
{}
mousse::uniformDensityHydrostaticPressureFvPatchScalarField::
uniformDensityHydrostaticPressureFvPatchScalarField
(
  const uniformDensityHydrostaticPressureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(ptf, iF),
  rho_(ptf.rho_),
  pRefValue_(ptf.pRefValue_),
  pRefPoint_(ptf.pRefPoint_)
{}
// Member Functions 
void mousse::uniformDensityHydrostaticPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const uniformDimensionedVectorField& g =
    db().lookupObject<uniformDimensionedVectorField>("g");
  operator==
  (
    pRefValue_
   + rho_*((g.value() & patch().Cf()) - (g.value() & pRefPoint_))
  );
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::uniformDensityHydrostaticPressureFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
  os.writeKeyword("rho") << rho_ << token::END_STATEMENT << nl;
  os.writeKeyword("pRefValue") << pRefValue_ << token::END_STATEMENT << nl;
  os.writeKeyword("pRefPoint") << pRefPoint_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    uniformDensityHydrostaticPressureFvPatchScalarField
  );
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotating_total_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"

// Constructors
mousse::rotatingTotalPressureFvPatchScalarField::
rotatingTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  totalPressureFvPatchScalarField{p, iF},
  omega_{}
{}

mousse::rotatingTotalPressureFvPatchScalarField::
rotatingTotalPressureFvPatchScalarField
(
  const rotatingTotalPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  totalPressureFvPatchScalarField{ptf, p, iF, mapper},
  omega_{ptf.omega_().clone().ptr()}
{}

mousse::rotatingTotalPressureFvPatchScalarField::
rotatingTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  totalPressureFvPatchScalarField{p, iF, dict},
  omega_{DataEntry<vector>::New("omega", dict)}
{}

mousse::rotatingTotalPressureFvPatchScalarField::
rotatingTotalPressureFvPatchScalarField
(
  const rotatingTotalPressureFvPatchScalarField& rtppsf
)
:
  totalPressureFvPatchScalarField{rtppsf},
  omega_{rtppsf.omega_().clone().ptr()}
{}

mousse::rotatingTotalPressureFvPatchScalarField::
rotatingTotalPressureFvPatchScalarField
(
  const rotatingTotalPressureFvPatchScalarField& rtppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  totalPressureFvPatchScalarField{rtppsf, iF},
  omega_{rtppsf.omega_().clone().ptr()}
{}

// Member Functions
void mousse::rotatingTotalPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  const vector om = omega_->value(t);
  vector axisHat = om/mag(om);
  tmp<vectorField> rotationVelocity =
    om ^ (patch().Cf() - axisHat*(axisHat & patch().Cf()));
  const vectorField Up
  (
    patch().lookupPatchField<volVectorField, vector>(UName())
   + rotationVelocity
  );
  totalPressureFvPatchScalarField::updateCoeffs(p0(), Up);
}
void mousse::rotatingTotalPressureFvPatchScalarField::write(Ostream& os) const
{
  totalPressureFvPatchScalarField::write(os);
  omega_->writeData(os);
}

namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  rotatingTotalPressureFvPatchScalarField
);
}

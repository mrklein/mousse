// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_flux_pressure_fv_patch_scalar_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"


// Constructors
mousse::fixedFluxPressureFvPatchScalarField::fixedFluxPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{p, iF},
  curTimeIndex_{-1}
{}


mousse::fixedFluxPressureFvPatchScalarField::fixedFluxPressureFvPatchScalarField
(
  const fixedFluxPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchScalarField{p, iF},
  curTimeIndex_{-1}
{
  patchType() = ptf.patchType();
  // Map gradient. Set unmapped values and overwrite with mapped ptf
  gradient() = 0.0;
  gradient().map(ptf.gradient(), mapper);
  // Evaluate the value field from the gradient if the internal field is valid
  if (notNull(iF) && iF.size()) {
    scalarField::operator=
    (
      //patchInternalField() + gradient()/patch().deltaCoeffs()
      // ***HGW Hack to avoid the construction of mesh.deltaCoeffs
      // which fails for AMI patches for some mapping operations
      patchInternalField() + gradient()*(patch().nf() & patch().delta())
    );
  }
}


mousse::fixedFluxPressureFvPatchScalarField::fixedFluxPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchScalarField{p, iF},
  curTimeIndex_{-1}
{
  if (dict.found("value") && dict.found("gradient")) {
    fvPatchField<scalar>::operator=
    (
      scalarField("value", dict, p.size())
    );
    gradient() = scalarField("gradient", dict, p.size());
  } else {
    fvPatchField<scalar>::operator=(patchInternalField());
    gradient() = 0.0;
  }
}


mousse::fixedFluxPressureFvPatchScalarField::fixedFluxPressureFvPatchScalarField
(
  const fixedFluxPressureFvPatchScalarField& wbppsf
)
:
  fixedGradientFvPatchScalarField{wbppsf},
  curTimeIndex_{-1}
{}


mousse::fixedFluxPressureFvPatchScalarField::fixedFluxPressureFvPatchScalarField
(
  const fixedFluxPressureFvPatchScalarField& wbppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{wbppsf, iF},
  curTimeIndex_{-1}
{}


// Member Functions
void mousse::fixedFluxPressureFvPatchScalarField::updateCoeffs
(
  const scalarField& snGradp
)
{
  if (updated()) {
    return;
  }
  curTimeIndex_ = this->db().time().timeIndex();
  gradient() = snGradp;
  fixedGradientFvPatchScalarField::updateCoeffs();
}


void mousse::fixedFluxPressureFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  if (curTimeIndex_ != this->db().time().timeIndex()) {
    FATAL_ERROR_IN("fixedFluxPressureFvPatchScalarField::updateCoeffs()")
      << "updateCoeffs(const scalarField& snGradp) MUST be called before"
       " updateCoeffs() or evaluate() to set the boundary gradient."
      << exit(FatalError);
  }
}


void mousse::fixedFluxPressureFvPatchScalarField::write(Ostream& os) const
{
  fixedGradientFvPatchScalarField::write(os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  fixedFluxPressureFvPatchScalarField
);

}


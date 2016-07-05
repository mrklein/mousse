// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_mesh.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(alphaContactAngleFvPatchScalarField, 0);
template<>
const char* mousse::NamedEnum
<
  mousse::alphaContactAngleFvPatchScalarField::limitControls,
  4
>::names[] =
{
  "none",
  "gradient",
  "zeroGradient",
  "alpha"
};

}

const mousse::NamedEnum
<
  mousse::alphaContactAngleFvPatchScalarField::limitControls,
  4
> mousse::alphaContactAngleFvPatchScalarField::limitControlNames_;


// Constructors 
mousse::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{p, iF},
  limit_{lcZeroGradient}
{}


mousse::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const alphaContactAngleFvPatchScalarField& acpsf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchScalarField{acpsf, p, iF, mapper},
  limit_{acpsf.limit_}
{}


mousse::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchScalarField{p, iF},
  limit_{limitControlNames_.read(dict.lookup("limit"))}
{
  if (dict.found("gradient")) {
    gradient() = scalarField("gradient", dict, p.size());
    fixedGradientFvPatchScalarField::updateCoeffs();
    fixedGradientFvPatchScalarField::evaluate();
  } else {
    fvPatchField<scalar>::operator=(patchInternalField());
    gradient() = 0.0;
  }
}


mousse::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const alphaContactAngleFvPatchScalarField& acpsf
)
:
  fixedGradientFvPatchScalarField{acpsf},
  limit_{acpsf.limit_}
{}


mousse::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const alphaContactAngleFvPatchScalarField& acpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{acpsf, iF},
  limit_{acpsf.limit_}
{}


// Member Functions 
void mousse::alphaContactAngleFvPatchScalarField::evaluate
(
  const Pstream::commsTypes
)
{
  if (limit_ == lcGradient) {
    gradient() =
      patch().deltaCoeffs()
      *(max(min(*this + gradient()/patch().deltaCoeffs(),
                scalar(1)),
            scalar(0)) - *this);
  } else if (limit_ == lcZeroGradient) {
    gradient() = 0.0;
  }
  fixedGradientFvPatchScalarField::evaluate();
  if (limit_ == lcAlpha) {
    scalarField::operator=(max(min(*this, scalar(1)), scalar(0)));
  }
}


void mousse::alphaContactAngleFvPatchScalarField::write
(
  Ostream& os
) const
{
  fixedGradientFvPatchScalarField::write(os);
  os.writeKeyword("limit")
    << limitControlNames_[limit_] << token::END_STATEMENT << nl;
}


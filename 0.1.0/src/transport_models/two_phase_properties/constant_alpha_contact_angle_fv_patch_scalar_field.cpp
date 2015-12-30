// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_mesh.hpp"
#include "fv_patch_field_mapper.hpp"
// Constructors 
mousse::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(p, iF),
  theta0_(0.0)
{}
mousse::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
  const constantAlphaContactAngleFvPatchScalarField& gcpsf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
  theta0_(gcpsf.theta0_)
{}
mousse::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  alphaContactAngleFvPatchScalarField(p, iF, dict),
  theta0_(readScalar(dict.lookup("theta0")))
{
  evaluate();
}
mousse::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
  const constantAlphaContactAngleFvPatchScalarField& gcpsf
)
:
  alphaContactAngleFvPatchScalarField(gcpsf),
  theta0_(gcpsf.theta0_)
{}
mousse::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
  const constantAlphaContactAngleFvPatchScalarField& gcpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, iF),
  theta0_(gcpsf.theta0_)
{}
// Member Functions 
mousse::tmp<mousse::scalarField>
mousse::constantAlphaContactAngleFvPatchScalarField::theta
(
  const fvPatchVectorField&,
  const fvsPatchVectorField&
) const
{
  return tmp<scalarField>(new scalarField(size(), theta0_));
}
void mousse::constantAlphaContactAngleFvPatchScalarField::write
(
  Ostream& os
) const
{
  alphaContactAngleFvPatchScalarField::write(os);
  os.writeKeyword("theta0") << theta0_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    constantAlphaContactAngleFvPatchScalarField
  );
}

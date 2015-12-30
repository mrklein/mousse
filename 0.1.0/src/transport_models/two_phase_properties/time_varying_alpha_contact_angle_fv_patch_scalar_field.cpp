// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_varying_alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_mesh.hpp"
#include "time.hpp"
// Constructors 
mousse::timeVaryingAlphaContactAngleFvPatchScalarField::
timeVaryingAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(p, iF),
  t0_(0.0),
  thetaT0_(0.0),
  te_(0.0),
  thetaTe_(0.0)
{}
mousse::timeVaryingAlphaContactAngleFvPatchScalarField::
timeVaryingAlphaContactAngleFvPatchScalarField
(
  const timeVaryingAlphaContactAngleFvPatchScalarField& gcpsf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
  t0_(gcpsf.t0_),
  thetaT0_(gcpsf.thetaT0_),
  te_(gcpsf.te_),
  thetaTe_(gcpsf.te_)
{}
mousse::timeVaryingAlphaContactAngleFvPatchScalarField::
timeVaryingAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  alphaContactAngleFvPatchScalarField(p, iF, dict),
  t0_(readScalar(dict.lookup("t0"))),
  thetaT0_(readScalar(dict.lookup("thetaT0"))),
  te_(readScalar(dict.lookup("te"))),
  thetaTe_(readScalar(dict.lookup("thetaTe")))
{
  evaluate();
}
mousse::timeVaryingAlphaContactAngleFvPatchScalarField::
timeVaryingAlphaContactAngleFvPatchScalarField
(
  const timeVaryingAlphaContactAngleFvPatchScalarField& gcpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, iF),
  t0_(gcpsf.t0_),
  thetaT0_(gcpsf.thetaT0_),
  te_(gcpsf.te_),
  thetaTe_(gcpsf.thetaTe_)
{}
// Member Functions 
mousse::tmp<mousse::scalarField>
mousse::timeVaryingAlphaContactAngleFvPatchScalarField::theta
(
  const fvPatchVectorField&,
  const fvsPatchVectorField&
) const
{
  scalar t = patch().boundaryMesh().mesh().time().value();
  scalar theta0 = thetaT0_;
  if (t < t0_)
  {
    theta0 = thetaT0_;
  }
  else if (t > te_)
  {
    theta0 = thetaTe_;
  }
  else
  {
    theta0 = thetaT0_ + (t - t0_)*(thetaTe_ - thetaT0_)/(te_ - t0_);
  }
  return tmp<scalarField>(new scalarField(size(), theta0));
}
void mousse::timeVaryingAlphaContactAngleFvPatchScalarField::write
(
  Ostream& os
) const
{
  alphaContactAngleFvPatchScalarField::write(os);
  os.writeKeyword("t0") << t0_ << token::END_STATEMENT << nl;
  os.writeKeyword("thetaT0") << thetaT0_ << token::END_STATEMENT << nl;
  os.writeKeyword("te") << te_ << token::END_STATEMENT << nl;
  os.writeKeyword("thetaTe") << thetaTe_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    timeVaryingAlphaContactAngleFvPatchScalarField
  );
}

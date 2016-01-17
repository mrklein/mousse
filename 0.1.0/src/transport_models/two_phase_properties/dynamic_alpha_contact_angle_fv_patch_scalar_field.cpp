// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_mesh.hpp"
// Constructors 
mousse::dynamicAlphaContactAngleFvPatchScalarField::
dynamicAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(p, iF),
  theta0_(0.0),
  uTheta_(0.0),
  thetaA_(0.0),
  thetaR_(0.0)
{}
mousse::dynamicAlphaContactAngleFvPatchScalarField::
dynamicAlphaContactAngleFvPatchScalarField
(
  const dynamicAlphaContactAngleFvPatchScalarField& gcpsf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
  theta0_(gcpsf.theta0_),
  uTheta_(gcpsf.uTheta_),
  thetaA_(gcpsf.thetaA_),
  thetaR_(gcpsf.thetaR_)
{}
mousse::dynamicAlphaContactAngleFvPatchScalarField::
dynamicAlphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  alphaContactAngleFvPatchScalarField(p, iF, dict),
  theta0_(readScalar(dict.lookup("theta0"))),
  uTheta_(readScalar(dict.lookup("uTheta"))),
  thetaA_(readScalar(dict.lookup("thetaA"))),
  thetaR_(readScalar(dict.lookup("thetaR")))
{
  evaluate();
}
mousse::dynamicAlphaContactAngleFvPatchScalarField::
dynamicAlphaContactAngleFvPatchScalarField
(
  const dynamicAlphaContactAngleFvPatchScalarField& gcpsf
)
:
  alphaContactAngleFvPatchScalarField(gcpsf),
  theta0_(gcpsf.theta0_),
  uTheta_(gcpsf.uTheta_),
  thetaA_(gcpsf.thetaA_),
  thetaR_(gcpsf.thetaR_)
{}
mousse::dynamicAlphaContactAngleFvPatchScalarField::
dynamicAlphaContactAngleFvPatchScalarField
(
  const dynamicAlphaContactAngleFvPatchScalarField& gcpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  alphaContactAngleFvPatchScalarField(gcpsf, iF),
  theta0_(gcpsf.theta0_),
  uTheta_(gcpsf.uTheta_),
  thetaA_(gcpsf.thetaA_),
  thetaR_(gcpsf.thetaR_)
{}
// Member Functions 
mousse::tmp<mousse::scalarField>
mousse::dynamicAlphaContactAngleFvPatchScalarField::theta
(
  const fvPatchVectorField& Up,
  const fvsPatchVectorField& nHat
) const
{
  if (uTheta_ < SMALL)
  {
    return tmp<scalarField>(new scalarField(size(), theta0_));
  }
  const vectorField nf(patch().nf());
  // Calculated the component of the velocity parallel to the wall
  vectorField Uwall(Up.patchInternalField() - Up);
  Uwall -= (nf & Uwall)*nf;
  // Find the direction of the interface parallel to the wall
  vectorField nWall(nHat - (nf & nHat)*nf);
  // Normalise nWall
  nWall /= (mag(nWall) + SMALL);
  // Calculate Uwall resolved normal to the interface parallel to
  // the interface
  scalarField uwall(nWall & Uwall);
  return theta0_ + (thetaA_ - thetaR_)*tanh(uwall/uTheta_);
}
void mousse::dynamicAlphaContactAngleFvPatchScalarField::write(Ostream& os) const
{
  alphaContactAngleFvPatchScalarField::write(os);
  os.writeKeyword("theta0") << theta0_ << token::END_STATEMENT << nl;
  os.writeKeyword("uTheta") << uTheta_ << token::END_STATEMENT << nl;
  os.writeKeyword("thetaA") << thetaA_ << token::END_STATEMENT << nl;
  os.writeKeyword("thetaR") << thetaR_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  dynamicAlphaContactAngleFvPatchScalarField
);
}

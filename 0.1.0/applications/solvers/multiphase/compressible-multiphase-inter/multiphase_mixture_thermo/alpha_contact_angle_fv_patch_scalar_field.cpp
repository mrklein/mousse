// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
namespace mousse
{
alphaContactAngleFvPatchScalarField::interfaceThetaProps::interfaceThetaProps
(
  Istream& is
)
:
  theta0_{readScalar(is)},
  uTheta_{readScalar(is)},
  thetaA_{readScalar(is)},
  thetaR_{readScalar(is)}
{}
Istream& operator>>
(
  Istream& is,
  alphaContactAngleFvPatchScalarField::interfaceThetaProps& tp
)
{
  is >> tp.theta0_ >> tp.uTheta_ >> tp.thetaA_ >> tp.thetaR_;
  return is;
}
Ostream& operator<<
(
  Ostream& os,
  const alphaContactAngleFvPatchScalarField::interfaceThetaProps& tp
)
{
  os << tp.theta0_ << token::SPACE
    << tp.uTheta_ << token::SPACE
    << tp.thetaA_ << token::SPACE
    << tp.thetaR_;
  return os;
}
// Constructors 
alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  zeroGradientFvPatchScalarField{p, iF}
{}
alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const alphaContactAngleFvPatchScalarField& gcpsf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  zeroGradientFvPatchScalarField{gcpsf, p, iF, mapper},
  thetaProps_{gcpsf.thetaProps_}
{}
alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  zeroGradientFvPatchScalarField{p, iF},
  thetaProps_{dict.lookup("thetaProperties")}
{
  evaluate();
}
alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
  const alphaContactAngleFvPatchScalarField& gcpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  zeroGradientFvPatchScalarField{gcpsf, iF},
  thetaProps_{gcpsf.thetaProps_}
{}
// Member Functions 
void alphaContactAngleFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  os.writeKeyword("thetaProperties") << thetaProps_
    << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  alphaContactAngleFvPatchScalarField
);
}  // namespace mousse

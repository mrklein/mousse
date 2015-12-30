// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "v2_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "wall_fv_patch.hpp"
namespace mousse
{
namespace RASModels
{
// Protected Member Functions 
void v2WallFunctionFvPatchScalarField::checkType()
{
  if (!isA<wallFvPatch>(patch()))
  {
    FatalErrorIn("v2WallFunctionFvPatchScalarField::checkType()")
      << "Invalid wall function specification" << nl
      << "    Patch type for patch " << patch().name()
      << " must be wall" << nl
      << "    Current patch type is " << patch().type() << nl << endl
      << abort(FatalError);
  }
}
void v2WallFunctionFvPatchScalarField::writeLocalEntries(Ostream& os) const
{
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
}
scalar v2WallFunctionFvPatchScalarField::yPlusLam
(
  const scalar kappa,
  const scalar E
)
{
  scalar ypl = 11.0;
  for (int i=0; i<10; i++)
  {
    ypl = log(max(E*ypl, 1))/kappa;
  }
  return ypl;
}
// Constructors 
v2WallFunctionFvPatchScalarField::v2WallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(p, iF),
  Cmu_(0.09),
  kappa_(0.41),
  E_(9.8),
  yPlusLam_(yPlusLam(kappa_, E_))
{
  checkType();
}
v2WallFunctionFvPatchScalarField::v2WallFunctionFvPatchScalarField
(
  const v2WallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<scalar>(ptf, p, iF, mapper),
  Cmu_(ptf.Cmu_),
  kappa_(ptf.kappa_),
  E_(ptf.E_),
  yPlusLam_(ptf.yPlusLam_)
{
  checkType();
}
v2WallFunctionFvPatchScalarField::v2WallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<scalar>(p, iF, dict),
  Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
  kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
  E_(dict.lookupOrDefault<scalar>("E", 9.8)),
  yPlusLam_(yPlusLam(kappa_, E_))
{
  checkType();
}
v2WallFunctionFvPatchScalarField::v2WallFunctionFvPatchScalarField
(
  const v2WallFunctionFvPatchScalarField& v2wfpsf
)
:
  fixedValueFvPatchField<scalar>(v2wfpsf),
  Cmu_(v2wfpsf.Cmu_),
  kappa_(v2wfpsf.kappa_),
  E_(v2wfpsf.E_),
  yPlusLam_(v2wfpsf.yPlusLam_)
{
  checkType();
}
v2WallFunctionFvPatchScalarField::v2WallFunctionFvPatchScalarField
(
  const v2WallFunctionFvPatchScalarField& v2wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(v2wfpsf, iF),
  Cmu_(v2wfpsf.Cmu_),
  kappa_(v2wfpsf.kappa_),
  E_(v2wfpsf.E_),
  yPlusLam_(v2wfpsf.yPlusLam_)
{
  checkType();
}
// Member Functions 
void v2WallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const label patchi = patch().index();
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  const scalarField& y = turbModel.y()[patchi];
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const scalar Cmu25 = pow025(Cmu_);
  scalarField& v2 = *this;
  // Set v2 wall values
  forAll(v2, faceI)
  {
    label faceCellI = patch().faceCells()[faceI];
    scalar uTau = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = uTau*y[faceI]/nuw[faceI];
    if (yPlus > yPlusLam_)
    {
      scalar Cv2 = 0.193;
      scalar Bv2 = -0.94;
      v2[faceI] = Cv2/kappa_*log(yPlus) + Bv2;
    }
    else
    {
      scalar Cv2 = 0.193;
      v2[faceI] = Cv2*pow4(yPlus);
    }
    v2[faceI] *= sqr(uTau);
  }
  fixedValueFvPatchField<scalar>::updateCoeffs();
  // TODO: perform averaging for cells sharing more than one boundary face
}
void v2WallFunctionFvPatchScalarField::evaluate
(
  const Pstream::commsTypes commsType
)
{
  fixedValueFvPatchField<scalar>::evaluate(commsType);
}
void v2WallFunctionFvPatchScalarField::write(Ostream& os) const
{
  writeLocalEntries(os);
  fixedValueFvPatchField<scalar>::write(os);
}
makePatchTypeField
(
  fvPatchScalarField,
  v2WallFunctionFvPatchScalarField
);
}  // namespace RASModels
}  // namespace mousse

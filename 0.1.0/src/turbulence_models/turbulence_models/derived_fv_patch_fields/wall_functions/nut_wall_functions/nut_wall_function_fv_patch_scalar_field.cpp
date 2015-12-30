// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nut_wall_function_fv_patch_scalar_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(nutWallFunctionFvPatchScalarField, 0);
// Protected Member Functions 
void nutWallFunctionFvPatchScalarField::checkType()
{
  if (!isA<wallFvPatch>(patch()))
  {
    FatalErrorIn("nutWallFunctionFvPatchScalarField::checkType()")
      << "Invalid wall function specification" << nl
      << "    Patch type for patch " << patch().name()
      << " must be wall" << nl
      << "    Current patch type is " << patch().type() << nl << endl
      << abort(FatalError);
  }
}
void nutWallFunctionFvPatchScalarField::writeLocalEntries(Ostream& os) const
{
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
}
// Constructors 
nutWallFunctionFvPatchScalarField::nutWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  Cmu_(0.09),
  kappa_(0.41),
  E_(9.8),
  yPlusLam_(yPlusLam(kappa_, E_))
{
  checkType();
}
nutWallFunctionFvPatchScalarField::nutWallFunctionFvPatchScalarField
(
  const nutWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  Cmu_(ptf.Cmu_),
  kappa_(ptf.kappa_),
  E_(ptf.E_),
  yPlusLam_(ptf.yPlusLam_)
{
  checkType();
}
nutWallFunctionFvPatchScalarField::nutWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF, dict),
  Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
  kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
  E_(dict.lookupOrDefault<scalar>("E", 9.8)),
  yPlusLam_(yPlusLam(kappa_, E_))
{
  checkType();
}
nutWallFunctionFvPatchScalarField::nutWallFunctionFvPatchScalarField
(
  const nutWallFunctionFvPatchScalarField& wfpsf
)
:
  fixedValueFvPatchScalarField(wfpsf),
  Cmu_(wfpsf.Cmu_),
  kappa_(wfpsf.kappa_),
  E_(wfpsf.E_),
  yPlusLam_(wfpsf.yPlusLam_)
{
  checkType();
}
nutWallFunctionFvPatchScalarField::nutWallFunctionFvPatchScalarField
(
  const nutWallFunctionFvPatchScalarField& wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(wfpsf, iF),
  Cmu_(wfpsf.Cmu_),
  kappa_(wfpsf.kappa_),
  E_(wfpsf.E_),
  yPlusLam_(wfpsf.yPlusLam_)
{
  checkType();
}
// Member Functions 
scalar nutWallFunctionFvPatchScalarField::yPlusLam
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
void nutWallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  operator==(calcNut());
  fixedValueFvPatchScalarField::updateCoeffs();
}
void nutWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  writeLocalEntries(os);
  writeEntry("value", os);
}
}  // namespace mousse

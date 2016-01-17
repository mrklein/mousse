// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "k_low_re_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "wall_fv_patch.hpp"
namespace mousse
{
// Private Member Functions 
void kLowReWallFunctionFvPatchScalarField::checkType()
{
  if (!isA<wallFvPatch>(patch()))
  {
    FATAL_ERROR_IN("kLowReWallFunctionFvPatchScalarField::checkType()")
      << "Invalid wall function specification" << nl
      << "    Patch type for patch " << patch().name()
      << " must be wall" << nl
      << "    Current patch type is " << patch().type() << nl << endl
      << abort(FatalError);
  }
}
// Protected Member Functions 
scalar kLowReWallFunctionFvPatchScalarField::yPlusLam
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
kLowReWallFunctionFvPatchScalarField::kLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>{p, iF},
  Cmu_{0.09},
  kappa_{0.41},
  E_{9.8},
  Ceps2_{1.9},
  yPlusLam_{yPlusLam(kappa_, E_)}
{
  checkType();
}
kLowReWallFunctionFvPatchScalarField::kLowReWallFunctionFvPatchScalarField
(
  const kLowReWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<scalar>{ptf, p, iF, mapper},
  Cmu_{ptf.Cmu_},
  kappa_{ptf.kappa_},
  E_{ptf.E_},
  Ceps2_{ptf.Ceps2_},
  yPlusLam_{ptf.yPlusLam_}
{
  checkType();
}
kLowReWallFunctionFvPatchScalarField::kLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<scalar>{p, iF, dict},
  Cmu_{dict.lookupOrDefault<scalar>("Cmu", 0.09)},
  kappa_{dict.lookupOrDefault<scalar>("kappa", 0.41)},
  E_{dict.lookupOrDefault<scalar>("E", 9.8)},
  Ceps2_{dict.lookupOrDefault<scalar>("Ceps2", 1.9)},
  yPlusLam_{yPlusLam(kappa_, E_)}
{
  checkType();
}
kLowReWallFunctionFvPatchScalarField::kLowReWallFunctionFvPatchScalarField
(
  const kLowReWallFunctionFvPatchScalarField& kwfpsf
)
:
  fixedValueFvPatchField<scalar>{kwfpsf},
  Cmu_{kwfpsf.Cmu_},
  kappa_{kwfpsf.kappa_},
  E_{kwfpsf.E_},
  Ceps2_{kwfpsf.Ceps2_},
  yPlusLam_{kwfpsf.yPlusLam_}
{
  checkType();
}
kLowReWallFunctionFvPatchScalarField::kLowReWallFunctionFvPatchScalarField
(
  const kLowReWallFunctionFvPatchScalarField& kwfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>{kwfpsf, iF},
  Cmu_{kwfpsf.Cmu_},
  kappa_{kwfpsf.kappa_},
  E_{kwfpsf.E_},
  Ceps2_{kwfpsf.Ceps2_},
  yPlusLam_{kwfpsf.yPlusLam_}
{
  checkType();
}
// Member Functions 
void kLowReWallFunctionFvPatchScalarField::updateCoeffs()
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
  scalarField& kw = *this;
  // Set k wall values
  FOR_ALL(kw, faceI)
  {
    label faceCellI = patch().faceCells()[faceI];
    scalar uTau = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = uTau*y[faceI]/nuw[faceI];
    if (yPlus > yPlusLam_)
    {
      scalar Ck = -0.416;
      scalar Bk = 8.366;
      kw[faceI] = Ck/kappa_*log(yPlus) + Bk;
    }
    else
    {
      scalar C = 11.0;
      scalar Cf = (1.0/sqr(yPlus + C) + 2.0*yPlus/pow3(C) - 1.0/sqr(C));
      kw[faceI] = 2400.0/sqr(Ceps2_)*Cf;
    }
    kw[faceI] *= sqr(uTau);
  }
  // Limit kw to avoid failure of the turbulence model due to division by kw
  kw = max(kw, SMALL);
  fixedValueFvPatchField<scalar>::updateCoeffs();
  // TODO: perform averaging for cells sharing more than one boundary face
}
void kLowReWallFunctionFvPatchScalarField::evaluate
(
  const Pstream::commsTypes commsType
)
{
  fixedValueFvPatchField<scalar>::evaluate(commsType);
}
void kLowReWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ceps2") << Ceps2_ << token::END_STATEMENT << nl;
  fixedValueFvPatchField<scalar>::write(os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  kLowReWallFunctionFvPatchScalarField
);
}  // namespace mousse

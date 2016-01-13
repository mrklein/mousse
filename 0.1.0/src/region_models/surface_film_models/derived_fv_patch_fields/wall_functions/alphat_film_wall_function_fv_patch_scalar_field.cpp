// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alphat_film_wall_function_fv_patch_scalar_field.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "surface_film_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mapped_wall_poly_patch.hpp"
#include "map_distribute.hpp"
namespace mousse
{
namespace compressible
{
namespace RASModels
{
// Constructors 
alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  B_{5.5},
  yPlusCrit_{11.05},
  Cmu_{0.09},
  kappa_{0.41},
  Prt_{0.85}
{}
alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
  const alphatFilmWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  B_{ptf.B_},
  yPlusCrit_{ptf.yPlusCrit_},
  Cmu_{ptf.Cmu_},
  kappa_{ptf.kappa_},
  Prt_{ptf.Prt_}
{}
alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF, dict},
  B_{dict.lookupOrDefault("B", 5.5)},
  yPlusCrit_{dict.lookupOrDefault("yPlusCrit", 11.05)},
  Cmu_{dict.lookupOrDefault("Cmu", 0.09)},
  kappa_{dict.lookupOrDefault("kappa", 0.41)},
  Prt_{dict.lookupOrDefault("Prt", 0.85)}
{}
alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
  const alphatFilmWallFunctionFvPatchScalarField& fwfpsf
)
:
  fixedValueFvPatchScalarField{fwfpsf},
  B_{fwfpsf.B_},
  yPlusCrit_{fwfpsf.yPlusCrit_},
  Cmu_{fwfpsf.Cmu_},
  kappa_{fwfpsf.kappa_},
  Prt_{fwfpsf.Prt_}
{}
alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
  const alphatFilmWallFunctionFvPatchScalarField& fwfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{fwfpsf, iF},
  B_{fwfpsf.B_},
  yPlusCrit_{fwfpsf.yPlusCrit_},
  Cmu_{fwfpsf.Cmu_},
  kappa_{fwfpsf.kappa_},
  Prt_{fwfpsf.Prt_}
{}
// Member Functions 
void alphatFilmWallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  typedef regionModels::surfaceFilmModels::surfaceFilmModel modelType;
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  bool foundFilm =
    db().time().foundObject<modelType>("surfaceFilmProperties");
  if (!foundFilm)
  {
    // do nothing on construction - film model doesn't exist yet
    return;
  }
  const label patchi = patch().index();
  // Retrieve phase change mass from surface film model
  const modelType& filmModel =
    db().time().lookupObject<modelType>("surfaceFilmProperties");
  const label filmPatchI = filmModel.regionPatchID(patchi);
  tmp<volScalarField> mDotFilm(filmModel.primaryMassTrans());
  scalarField mDotFilmp = mDotFilm().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, mDotFilmp);
  // Retrieve RAS turbulence model
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  const scalarField& y = turbModel.y()[patchi];
  const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tmuw = turbModel.mu(patchi);
  const scalarField& muw = tmuw();
  const tmp<scalarField> talpha = turbModel.alpha(patchi);
  const scalarField& alphaw = talpha();
  const scalar Cmu25 = pow(Cmu_, 0.25);
  // Populate alphat field values
  scalarField& alphat = *this;
  FOR_ALL(alphat, faceI)
  {
    label faceCellI = patch().faceCells()[faceI];
    scalar uTau = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = y[faceI]*uTau/(muw[faceI]/rhow[faceI]);
    scalar Pr = muw[faceI]/alphaw[faceI];
    scalar factor = 0.0;
    scalar mStar = mDotFilmp[faceI]/(y[faceI]*uTau);
    if (yPlus > yPlusCrit_)
    {
      scalar expTerm = exp(min(50.0, yPlusCrit_*mStar*Pr));
      scalar yPlusRatio = yPlus/yPlusCrit_;
      scalar powTerm = mStar*Prt_/kappa_;
      factor =
        mStar/(expTerm*(pow(yPlusRatio, powTerm)) - 1.0 + ROOTVSMALL);
    }
    else
    {
      scalar expTerm = exp(min(50.0, yPlus*mStar*Pr));
      factor = mStar/(expTerm - 1.0 + ROOTVSMALL);
    }
    scalar dx = patch().deltaCoeffs()[faceI];
    scalar alphaEff = dx*rhow[faceI]*uTau*factor;
    alphat[faceI] = max(alphaEff - alphaw[faceI], 0.0);
  }
  // Restore tag
  UPstream::msgType() = oldTag;
  fixedValueFvPatchScalarField::updateCoeffs();
}
// Member Functions 
void alphatFilmWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
  os.writeKeyword("yPlusCrit") << yPlusCrit_ << token::END_STATEMENT << nl;
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("Prt") << Prt_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  alphatFilmWallFunctionFvPatchScalarField
);
}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse

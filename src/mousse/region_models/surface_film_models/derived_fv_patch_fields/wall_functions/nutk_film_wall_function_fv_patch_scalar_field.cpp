// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nutk_film_wall_function_fv_patch_scalar_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_film_model.hpp"
#include "mapped_wall_poly_patch.hpp"
#include "map_distribute.hpp"


namespace mousse {
namespace compressible {
namespace RASModels {

// Protected Member Functions 
tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::calcUTau
(
  const scalarField& magGradU
) const
{
  tmp<scalarField> tuTau{new scalarField{patch().size(), 0.0}};
  scalarField& uTau = tuTau();
  typedef regionModels::surfaceFilmModels::surfaceFilmModel modelType;
  bool foundFilm =
    db().time().foundObject<modelType>("surfaceFilmProperties");
  if (!foundFilm) {
    // do nothing on construction - film model doesn't exist yet
    return tuTau;
  }
  const label patchi = patch().index();
  // Retrieve phase change mass from surface film model
  const modelType& filmModel =
    db().time().lookupObject<modelType>("surfaceFilmProperties");
  const label filmPatchI = filmModel.regionPatchID(patchi);
  tmp<volScalarField> mDotFilm{filmModel.primaryMassTrans()};
  scalarField mDotFilmp = mDotFilm().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, mDotFilmp);
  // Retrieve RAS turbulence model
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
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
  const scalar Cmu25 = pow(Cmu_, 0.25);
  FOR_ALL(uTau, faceI) {
    label faceCellI = patch().faceCells()[faceI];
    scalar ut = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = y[faceI]*ut/nuw[faceI];
    scalar mStar = mDotFilmp[faceI]/(y[faceI]*ut);
    scalar factor = 0.0;
    if (yPlus > yPlusCrit_) {
      scalar expTerm = exp(min(50.0, B_*mStar));
      scalar powTerm = pow(yPlus, mStar/kappa_);
      factor = mStar/(expTerm*powTerm - 1.0 + ROOTVSMALL);
    } else {
      scalar expTerm = exp(min(50.0, mStar));
      factor = mStar/(expTerm*yPlus - 1.0 + ROOTVSMALL);
    }
    uTau[faceI] = sqrt(max(0, magGradU[faceI]*ut*factor));
  }
  return tuTau;
}


tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::calcNut() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField magGradU{mag(Uw.snGrad())};
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return
    max
    (
      scalar(0),
      sqr(calcUTau(magGradU))/(magGradU + ROOTVSMALL) - nuw
    );
}


// Constructors 
nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutkWallFunctionFvPatchScalarField{p, iF},
  B_{5.5},
  yPlusCrit_{11.05}
{}


nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
(
  const nutkFilmWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutkWallFunctionFvPatchScalarField{ptf, p, iF, mapper},
  B_{5.5},
  yPlusCrit_{11.05}
{}


nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutkWallFunctionFvPatchScalarField{p, iF, dict},
  B_{dict.lookupOrDefault("B", 5.5)},
  yPlusCrit_{dict.lookupOrDefault("yPlusCrit", 11.05)}
{}


nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
(
  const nutkFilmWallFunctionFvPatchScalarField& wfpsf
)
:
  nutkWallFunctionFvPatchScalarField{wfpsf},
  B_{wfpsf.B_},
  yPlusCrit_{wfpsf.yPlusCrit_}
{}


nutkFilmWallFunctionFvPatchScalarField::nutkFilmWallFunctionFvPatchScalarField
(
  const nutkFilmWallFunctionFvPatchScalarField& wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutkWallFunctionFvPatchScalarField{wfpsf, iF},
  B_{wfpsf.B_},
  yPlusCrit_{wfpsf.yPlusCrit_}
{}


// Member Functions 
tmp<scalarField> nutkFilmWallFunctionFvPatchScalarField::yPlus() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField& y = turbModel.y()[patchi];
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return y*calcUTau(mag(Uw.snGrad()))/nuw;
}


void nutkFilmWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  writeLocalEntries(os);
  os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
  os.writeKeyword("yPlusCrit") << yPlusCrit_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}


MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutkFilmWallFunctionFvPatchScalarField
);

}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse


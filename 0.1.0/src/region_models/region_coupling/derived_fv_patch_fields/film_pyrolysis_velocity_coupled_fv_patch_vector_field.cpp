// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_pyrolysis_velocity_coupled_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "pyrolysis_model.hpp"
#include "surface_film_model.hpp"
// Constructors 
mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::
filmPyrolysisVelocityCoupledFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  filmRegionName_{"surfaceFilmProperties"},
  pyrolysisRegionName_{"pyrolysisProperties"},
  phiName_{"phi"},
  rhoName_{"rho"}
{}
mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::
filmPyrolysisVelocityCoupledFvPatchVectorField
(
  const filmPyrolysisVelocityCoupledFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  filmRegionName_{ptf.filmRegionName_},
  pyrolysisRegionName_{ptf.pyrolysisRegionName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}
mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::
filmPyrolysisVelocityCoupledFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  filmRegionName_
  {
    dict.lookupOrDefault<word>("filmRegion", "surfaceFilmProperties")
  },
  pyrolysisRegionName_
  {
    dict.lookupOrDefault<word>("pyrolysisRegion", "pyrolysisProperties")
  },
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::
filmPyrolysisVelocityCoupledFvPatchVectorField
(
  const filmPyrolysisVelocityCoupledFvPatchVectorField& fpvpvf
)
:
  fixedValueFvPatchVectorField{fpvpvf},
  filmRegionName_{fpvpvf.filmRegionName_},
  pyrolysisRegionName_{fpvpvf.pyrolysisRegionName_},
  phiName_{fpvpvf.phiName_},
  rhoName_{fpvpvf.rhoName_}
{}
mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::
filmPyrolysisVelocityCoupledFvPatchVectorField
(
  const filmPyrolysisVelocityCoupledFvPatchVectorField& fpvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{fpvpvf, iF},
  filmRegionName_{fpvpvf.filmRegionName_},
  pyrolysisRegionName_{fpvpvf.pyrolysisRegionName_},
  phiName_{fpvpvf.phiName_},
  rhoName_{fpvpvf.rhoName_}
{}
// Member Functions 
void mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  typedef regionModels::surfaceFilmModels::surfaceFilmModel filmModelType;
  typedef regionModels::pyrolysisModels::pyrolysisModel pyrModelType;
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  bool foundFilm = db().time().foundObject<filmModelType>(filmRegionName_);
  bool foundPyrolysis =
    db().time().foundObject<pyrModelType>(pyrolysisRegionName_);
  if (!foundFilm || !foundPyrolysis)
  {
    // do nothing on construction - film model doesn't exist yet
    return;
  }
  vectorField& Up = *this;
  const label patchI = patch().index();
  // Retrieve film model
  const filmModelType& filmModel =
    db().time().lookupObject<filmModelType>(filmRegionName_);
  const label filmPatchI = filmModel.regionPatchID(patchI);
  scalarField alphaFilm = filmModel.alpha().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, alphaFilm);
  vectorField UFilm = filmModel.Us().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, UFilm);
  // Retrieve pyrolysis model
  const pyrModelType& pyrModel =
    db().time().lookupObject<pyrModelType>(pyrolysisRegionName_);
  const label pyrPatchI = pyrModel.regionPatchID(patchI);
  scalarField phiPyr = pyrModel.phiGas().boundaryField()[pyrPatchI];
  pyrModel.toPrimary(pyrPatchI, phiPyr);
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    // do nothing
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    phiPyr /= rhop;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "filmPyrolysisVelocityCoupledFvPatchVectorField::updateCoeffs()"
    )
    << "Unable to process flux field phi with dimensions "
    << phi.dimensions() << nl
    << "    on patch " << patch().name()
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
  const scalarField UAvePyr(-phiPyr/patch().magSf());
  const vectorField& nf = patch().nf();
  // Evaluate velocity
  Up = alphaFilm*UFilm + (1.0 - alphaFilm)*UAvePyr*nf;
  // Restore tag
  UPstream::msgType() = oldTag;
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::filmPyrolysisVelocityCoupledFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>
  (
    os,
    "filmRegion",
    "surfaceFilmProperties",
    filmRegionName_
  );
  writeEntryIfDifferent<word>
  (
    os,
    "pyrolysisRegion",
    "pyrolysisProperties",
    pyrolysisRegionName_
  );
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  filmPyrolysisVelocityCoupledFvPatchVectorField
);
}

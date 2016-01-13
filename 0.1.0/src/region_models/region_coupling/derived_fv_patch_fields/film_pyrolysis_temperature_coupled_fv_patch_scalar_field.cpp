// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_pyrolysis_temperature_coupled_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "pyrolysis_model.hpp"
#include "surface_film_model.hpp"
// Constructors 
mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::
filmPyrolysisTemperatureCoupledFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  filmRegionName_{"surfaceFilmProperties"},
  pyrolysisRegionName_{"pyrolysisProperties"},
  phiName_{"phi"},
  rhoName_{"rho"}
{}
mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::
filmPyrolysisTemperatureCoupledFvPatchScalarField
(
  const filmPyrolysisTemperatureCoupledFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  filmRegionName_{ptf.filmRegionName_},
  pyrolysisRegionName_{ptf.pyrolysisRegionName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}
mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::
filmPyrolysisTemperatureCoupledFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
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
  fvPatchScalarField::operator=(scalarField("value", dict, p.size()));
}
mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::
filmPyrolysisTemperatureCoupledFvPatchScalarField
(
  const filmPyrolysisTemperatureCoupledFvPatchScalarField& fptpsf
)
:
  fixedValueFvPatchScalarField{fptpsf},
  filmRegionName_{fptpsf.filmRegionName_},
  pyrolysisRegionName_{fptpsf.pyrolysisRegionName_},
  phiName_{fptpsf.phiName_},
  rhoName_{fptpsf.rhoName_}
{}
mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::
filmPyrolysisTemperatureCoupledFvPatchScalarField
(
  const filmPyrolysisTemperatureCoupledFvPatchScalarField& fptpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{fptpsf, iF},
  filmRegionName_{fptpsf.filmRegionName_},
  pyrolysisRegionName_{fptpsf.pyrolysisRegionName_},
  phiName_{fptpsf.phiName_},
  rhoName_{fptpsf.rhoName_}
{}
// Member Functions 
void mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::updateCoeffs()
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
  bool filmOk = db().time().foundObject<filmModelType>(filmRegionName_);
  bool pyrOk = db().time().foundObject<pyrModelType>(pyrolysisRegionName_);
  if (!filmOk || !pyrOk)
  {
    // do nothing on construction - film model doesn't exist yet
    return;
  }
  scalarField& Tp = *this;
  const label patchI = patch().index();
  // Retrieve film model
  const filmModelType& filmModel =
    db().time().lookupObject<filmModelType>(filmRegionName_);
  const label filmPatchI = filmModel.regionPatchID(patchI);
  scalarField alphaFilm = filmModel.alpha().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, alphaFilm);
  scalarField TFilm = filmModel.Ts().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, TFilm);
  // Retrieve pyrolysis model
  const pyrModelType& pyrModel =
    db().time().lookupObject<pyrModelType>(pyrolysisRegionName_);
  const label pyrPatchI = pyrModel.regionPatchID(patchI);
  scalarField TPyr = pyrModel.T().boundaryField()[pyrPatchI];
  pyrModel.toPrimary(pyrPatchI, TPyr);
  // Evaluate temperature
  Tp = alphaFilm*TFilm + (1.0 - alphaFilm)*TPyr;
  // Restore tag
  UPstream::msgType() = oldTag;
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
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
  fvPatchScalarField,
  filmPyrolysisTemperatureCoupledFvPatchScalarField
);
}

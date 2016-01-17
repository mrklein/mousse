// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_pyrolysis_radiative_coupled_mixed_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mapped_patch_base.hpp"
namespace mousse
{
// Private Member Functions 
const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::filmModelType&
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
filmModel() const
{
  HashTable<const filmModelType*> models
    = db().time().lookupClass<filmModelType>();
  FOR_ALL_CONST_ITER(HashTable<const filmModelType*>, models, iter)
  {
    if (iter()->regionMesh().name() == filmRegionName_)
    {
      return *iter();
    }
  }
  DynamicList<word> modelNames;
  FOR_ALL_CONST_ITER(HashTable<const filmModelType*>, models, iter)
  {
    modelNames.append(iter()->regionMesh().name());
  }
  FATAL_ERROR_IN
  (
    "const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
    "filmModelType& "
    "filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
    "filmModel() const"
  )
  << "Unable to locate film region " << filmRegionName_
  << ".  Available regions include: " << modelNames
  << abort(FatalError);
  return **models.begin();
}
const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
pyrolysisModelType&
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
pyrModel() const
{
  HashTable<const pyrolysisModelType*> models =
    db().time().lookupClass<pyrolysisModelType>();
  FOR_ALL_CONST_ITER(HashTable<const pyrolysisModelType*>, models, iter)
  {
    if (iter()->regionMesh().name() == pyrolysisRegionName_)
    {
      return *iter();
    }
  }
  DynamicList<word> modelNames;
  FOR_ALL_CONST_ITER(HashTable<const pyrolysisModelType*>, models, iter)
  {
    modelNames.append(iter()->regionMesh().name());
  }
  FATAL_ERROR_IN
  (
    "const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
    "pyrolysisModelType& "
    "filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
    "pyrModel() const"
  )
  << "Unable to locate pyrolysis region " << pyrolysisRegionName_
  << ".  Available regions include: " << modelNames
  << abort(FatalError);
  return **models.begin();
}
// Constructors 
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF},
  temperatureCoupledBase{patch(), "undefined", "undefined", "undefined-K"},
  filmRegionName_{"surfaceFilmProperties"},
  pyrolysisRegionName_{"pyrolysisProperties"},
  TnbrName_{"undefined-Tnbr"},
  QrName_{"undefined-Qr"},
  convectiveScaling_{1.0},
  filmDeltaDry_{0.0},
  filmDeltaWet_{0.0}
{
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 1.0;
}
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
(
  const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField& psf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{psf, p, iF, mapper},
  temperatureCoupledBase{patch(), psf},
  filmRegionName_{psf.filmRegionName_},
  pyrolysisRegionName_{psf.pyrolysisRegionName_},
  TnbrName_{psf.TnbrName_},
  QrName_{psf.QrName_},
  convectiveScaling_{psf.convectiveScaling_},
  filmDeltaDry_{psf.filmDeltaDry_},
  filmDeltaWet_{psf.filmDeltaWet_}
{}
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF},
  temperatureCoupledBase{patch(), dict},
  filmRegionName_
  {
    dict.lookupOrDefault<word>("filmRegion", "surfaceFilmProperties")
  },
  pyrolysisRegionName_
  {
    dict.lookupOrDefault<word>("pyrolysisRegion", "pyrolysisProperties")
  },
  TnbrName_{dict.lookup("Tnbr")},
  QrName_{dict.lookup("Qr")},
  convectiveScaling_{dict.lookupOrDefault<scalar>("convectiveScaling", 1.0)},
  filmDeltaDry_{readScalar(dict.lookup("filmDeltaDry"))},
  filmDeltaWet_{readScalar(dict.lookup("filmDeltaWet"))}
{
  if (!isA<mappedPatchBase>(this->patch().patch()))
  {
    FATAL_ERROR_IN
    (
      "filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
      "filmPyrolysisRadiativeCoupledMixedFvPatchScalarField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<scalar, volMesh>& iF,\n"
      "    const dictionary& dict\n"
      ")\n"
    )
    << "\n    patch type '" << p.type()
    << "' not type '" << mappedPatchBase::typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
  fvPatchScalarField::operator=(scalarField("value", dict, p.size()));
  if (dict.found("refValue"))
  {
    // Full restart
    refValue() = scalarField("refValue", dict, p.size());
    refGrad() = scalarField("refGradient", dict, p.size());
    valueFraction() = scalarField("valueFraction", dict, p.size());
  }
  else
  {
    // Start from user entered data. Assume fixedValue.
    refValue() = *this;
    refGrad() = 0.0;
    valueFraction() = 1.0;
  }
}
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::
filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
(
  const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField& psf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{psf, iF},
  temperatureCoupledBase{patch(), psf},
  filmRegionName_{psf.filmRegionName_},
  pyrolysisRegionName_{psf.pyrolysisRegionName_},
  TnbrName_{psf.TnbrName_},
  QrName_{psf.QrName_},
  convectiveScaling_{psf.convectiveScaling_},
  filmDeltaDry_{psf.filmDeltaDry_},
  filmDeltaWet_{psf.filmDeltaWet_}
{}
// Member Functions 
void filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Get the coupling information from the mappedPatchBase
  const mappedPatchBase& mpp =
    refCast<const mappedPatchBase>(patch().patch());
  const label patchI = patch().index();
  const label nbrPatchI = mpp.samplePolyPatch().index();
  const polyMesh& mesh = patch().boundaryMesh().mesh();
  const polyMesh& nbrMesh = mpp.sampleMesh();
  const fvPatch& nbrPatch =
    refCast<const fvMesh>(nbrMesh).boundary()[nbrPatchI];
  scalarField intFld(patchInternalField());
  const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField&
    nbrField =
    refCast
    <
      const filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
    >
    (
      nbrPatch.lookupPatchField<volScalarField, scalar>(TnbrName_)
    );
  // Swap to obtain full local values of neighbour internal field
  scalarField nbrIntFld(nbrField.patchInternalField());
  mpp.distribute(nbrIntFld);
  scalarField& Tp = *this;
  const scalarField K(this->kappa(*this));
  const scalarField nbrK(nbrField.kappa(*this));
  // Swap to obtain full local values of neighbour K*delta
  scalarField KDeltaNbr(nbrK*nbrPatch.deltaCoeffs());
  mpp.distribute(KDeltaNbr);
  scalarField myKDelta(K*patch().deltaCoeffs());
  scalarList Tfilm(patch().size(), 0.0);
  scalarList htcwfilm(patch().size(), 0.0);
  scalarList filmDelta(patch().size(), 0.0);
  const pyrolysisModelType& pyrolysis = pyrModel();
  const filmModelType& film = filmModel();
  // Obtain Rad heat (Qr)
  scalarField Qr(patch().size(), 0.0);
  label coupledPatchI = -1;
  if (pyrolysisRegionName_ == mesh.name())
  {
    coupledPatchI = patchI;
    if (QrName_ != "none")
    {
      Qr = nbrPatch.lookupPatchField<volScalarField, scalar>(QrName_);
      mpp.distribute(Qr);
    }
  }
  else if (pyrolysis.primaryMesh().name() == mesh.name())
  {
    coupledPatchI = nbrPatch.index();
    if (QrName_ != "none")
    {
      Qr = patch().lookupPatchField<volScalarField, scalar>(QrName_);
    }
  }
  else
  {
    FATAL_ERROR_IN
    (
      "void filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::"
      "updateCoeffs()"
    )
    << type() << " condition is intended to be applied to either the "
    << "primary or pyrolysis regions only"
    << exit(FatalError);
  }
  const label filmPatchI = pyrolysis.nbrCoupledPatchID(film, coupledPatchI);
  const scalarField htcw(film.htcw().h()().boundaryField()[filmPatchI]);
  // Obtain htcw
  htcwfilm =
    pyrolysis.mapRegionPatchField
    (
      film,
      coupledPatchI,
      filmPatchI,
      htcw,
      true
    );
  // Obtain Tfilm at the boundary through Ts.
  // NOTE: Tf is not good as at the boundary it will retrieve Tp
  Tfilm = film.Ts().boundaryField()[filmPatchI];
  film.toPrimary(filmPatchI, Tfilm);
  // Obtain delta
  filmDelta =
    pyrolysis.mapRegionPatchField<scalar>
    (
      film,
      "deltaf",
      coupledPatchI,
      true
    );
  // Estimate wetness of the film (1: wet , 0: dry)
  scalarField ratio
  (
    min
    (
      max
      (
        (filmDelta - filmDeltaDry_)/(filmDeltaWet_ - filmDeltaDry_),
        scalar(0.0)
      ),
      scalar(1.0)
    )
  );
  scalarField qConv(ratio*htcwfilm*(Tfilm - Tp)*convectiveScaling_);
  scalarField qRad((1.0 - ratio)*Qr);
  scalarField alpha(KDeltaNbr - (qRad + qConv)/Tp);
  valueFraction() = alpha/(alpha + (1.0 - ratio)*myKDelta);
  refValue() = ratio*Tfilm + (1.0 - ratio)*(KDeltaNbr*nbrIntFld)/alpha;
  mixedFvPatchScalarField::updateCoeffs();
  if (debug)
  {
    scalar Qc = gSum(qConv*patch().magSf());
    scalar Qr = gSum(qRad*patch().magSf());
    scalar Qt = gSum((qConv + qRad)*patch().magSf());
    Info<< mesh.name() << ':'
      << patch().name() << ':'
      << this->dimensionedInternalField().name() << " <- "
      << nbrMesh.name() << ':'
      << nbrPatch.name() << ':'
      << this->dimensionedInternalField().name() << " :" << nl
      << "     convective heat[W] : " << Qc << nl
      << "     radiative heat [W] : " << Qr << nl
      << "     total heat     [W] : " << Qt << nl
      << "     wall temperature "
      << " min:" << gMin(*this)
      << " max:" << gMax(*this)
      << " avg:" << gAverage(*this)
      << endl;
  }
}
void filmPyrolysisRadiativeCoupledMixedFvPatchScalarField::write
(
  Ostream& os
) const
{
  mixedFvPatchScalarField::write(os);
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
  os.writeKeyword("Tnbr")<< TnbrName_ << token::END_STATEMENT << nl;
  os.writeKeyword("Qr")<< QrName_ << token::END_STATEMENT << nl;
  os.writeKeyword("convectiveScaling") << convectiveScaling_
    << token::END_STATEMENT << nl;
  os.writeKeyword("filmDeltaDry") << filmDeltaDry_
    << token::END_STATEMENT << nl;
  os.writeKeyword("filmDeltaWet") << filmDeltaWet_
    << token::END_STATEMENT << endl;
  temperatureCoupledBase::write(os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  filmPyrolysisRadiativeCoupledMixedFvPatchScalarField
);
}  // namespace mousse

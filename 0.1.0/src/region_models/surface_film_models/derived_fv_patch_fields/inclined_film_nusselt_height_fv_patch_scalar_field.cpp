// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inclined_film_nusselt_height_fv_patch_scalar_field.hpp"
#include "vol_fields.hpp"
#include "kinematic_single_layer.hpp"
#include "add_to_run_time_selection_table.hpp"
// Constructors 
mousse::inclinedFilmNusseltHeightFvPatchScalarField::
inclinedFilmNusseltHeightFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  GammaMean_(),
  a_(),
  omega_()
{}
mousse::inclinedFilmNusseltHeightFvPatchScalarField::
inclinedFilmNusseltHeightFvPatchScalarField
(
  const inclinedFilmNusseltHeightFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  GammaMean_(ptf.GammaMean_().clone().ptr()),
  a_(ptf.a_().clone().ptr()),
  omega_(ptf.omega_().clone().ptr())
{}
mousse::inclinedFilmNusseltHeightFvPatchScalarField::
inclinedFilmNusseltHeightFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF),
  GammaMean_(DataEntry<scalar>::New("GammaMean", dict)),
  a_(DataEntry<scalar>::New("a", dict)),
  omega_(DataEntry<scalar>::New("omega", dict))
{
  fvPatchScalarField::operator=(scalarField("value", dict, p.size()));
}
mousse::inclinedFilmNusseltHeightFvPatchScalarField::
inclinedFilmNusseltHeightFvPatchScalarField
(
  const inclinedFilmNusseltHeightFvPatchScalarField& wmfrhpsf
)
:
  fixedValueFvPatchScalarField(wmfrhpsf),
  GammaMean_(wmfrhpsf.GammaMean_().clone().ptr()),
  a_(wmfrhpsf.a_().clone().ptr()),
  omega_(wmfrhpsf.omega_().clone().ptr())
{}
mousse::inclinedFilmNusseltHeightFvPatchScalarField::
inclinedFilmNusseltHeightFvPatchScalarField
(
  const inclinedFilmNusseltHeightFvPatchScalarField& wmfrhpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(wmfrhpsf, iF),
  GammaMean_(wmfrhpsf.GammaMean_().clone().ptr()),
  a_(wmfrhpsf.a_().clone().ptr()),
  omega_(wmfrhpsf.omega_().clone().ptr())
{}
// Member Functions 
void mousse::inclinedFilmNusseltHeightFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const label patchI = patch().index();
  // retrieve the film region from the database
  const regionModels::regionModel& region =
    db().time().lookupObject<regionModels::regionModel>
    (
      "surfaceFilmProperties"
    );
  const regionModels::surfaceFilmModels::kinematicSingleLayer& film =
    dynamic_cast
    <
      const regionModels::surfaceFilmModels::kinematicSingleLayer&
    >(region);
  // calculate the vector tangential to the patch
  // note: normal pointing into the domain
  const vectorField n(-patch().nf());
  // TODO: currently re-evaluating the entire gTan field to return this patch
  const scalarField gTan(film.gTan()().boundaryField()[patchI] & n);
  if (patch().size() && (max(mag(gTan)) < SMALL))
  {
    WarningIn
    (
      "void mousse::inclinedFilmNusseltHeightFvPatchScalarField::"
      "updateCoeffs()"
    )
      << "Tangential gravity component is zero.  This boundary condition "
      << "is designed to operate on patches inclined with respect to "
      << "gravity"
      << endl;
  }
  const volVectorField& nHat = film.nHat();
  const vectorField nHatp(nHat.boundaryField()[patchI].patchInternalField());
  vectorField nTan(nHatp ^ n);
  nTan /= mag(nTan) + ROOTVSMALL;
  // calculate distance in patch tangential direction
  const vectorField& Cf = patch().Cf();
  scalarField d(nTan & Cf);
  // calculate the wavy film height
  const scalar t = db().time().timeOutputValue();
  const scalar GMean = GammaMean_->value(t);
  const scalar a = a_->value(t);
  const scalar omega = omega_->value(t);
  const scalarField G(GMean + a*sin(omega*constant::mathematical::twoPi*d));
  const volScalarField& mu = film.mu();
  const scalarField mup(mu.boundaryField()[patchI].patchInternalField());
  const volScalarField& rho = film.rho();
  const scalarField rhop(rho.boundaryField()[patchI].patchInternalField());
  const scalarField Re(max(G, scalar(0.0))/mup);
  operator==
  (
    pow(3.0*sqr(mup/rhop)/(gTan + ROOTVSMALL), 0.333)*pow(Re, 0.333)
  );
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::inclinedFilmNusseltHeightFvPatchScalarField::write
(
  Ostream& os
) const
{
  fixedValueFvPatchScalarField::write(os);
  GammaMean_->writeData(os);
  a_->writeData(os);
  omega_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    inclinedFilmNusseltHeightFvPatchScalarField
  );
}

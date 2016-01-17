// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inclined_film_nusselt_inlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "kinematic_single_layer.hpp"
#include "add_to_run_time_selection_table.hpp"
// Constructors 
mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::
inclinedFilmNusseltInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  GammaMean_{},
  a_{},
  omega_{}
{}
mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::
inclinedFilmNusseltInletVelocityFvPatchVectorField
(
  const inclinedFilmNusseltInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  GammaMean_{ptf.GammaMean_().clone().ptr()},
  a_{ptf.a_().clone().ptr()},
  omega_{ptf.omega_().clone().ptr()}
{}
mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::
inclinedFilmNusseltInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  GammaMean_{DataEntry<scalar>::New("GammaMean", dict)},
  a_{DataEntry<scalar>::New("a", dict)},
  omega_{DataEntry<scalar>::New("omega", dict)}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::
inclinedFilmNusseltInletVelocityFvPatchVectorField
(
  const inclinedFilmNusseltInletVelocityFvPatchVectorField& fmfrpvf
)
:
  fixedValueFvPatchVectorField{fmfrpvf},
  GammaMean_{fmfrpvf.GammaMean_().clone().ptr()},
  a_{fmfrpvf.a_().clone().ptr()},
  omega_{fmfrpvf.omega_().clone().ptr()}
{}
mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::
inclinedFilmNusseltInletVelocityFvPatchVectorField
(
  const inclinedFilmNusseltInletVelocityFvPatchVectorField& fmfrpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{fmfrpvf, iF},
  GammaMean_{fmfrpvf.GammaMean_().clone().ptr()},
  a_{fmfrpvf.a_().clone().ptr()},
  omega_{fmfrpvf.omega_().clone().ptr()}
{}
// Member Functions 
void mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::updateCoeffs()
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
    WARNING_IN
    (
      "void mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::"
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
  operator==(n*pow(gTan*mup/(3.0*rhop), 0.333)*pow(Re, 0.666));
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  GammaMean_->writeData(os);
  a_->writeData(os);
  omega_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  inclinedFilmNusseltInletVelocityFvPatchVectorField
);
}

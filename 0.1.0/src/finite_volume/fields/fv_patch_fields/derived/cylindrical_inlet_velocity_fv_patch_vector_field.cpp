// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cylindrical_inlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"
#include "mathematical_constants.hpp"
#include "time.hpp"

// Constructors
mousse::cylindricalInletVelocityFvPatchVectorField::
cylindricalInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{p, iF},
  centre_{pTraits<vector>::zero},
  axis_{pTraits<vector>::zero},
  axialVelocity_{},
  radialVelocity_{},
  rpm_{}
{}

mousse::cylindricalInletVelocityFvPatchVectorField::
cylindricalInletVelocityFvPatchVectorField
(
  const cylindricalInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>{ptf, p, iF, mapper},
  centre_{ptf.centre_},
  axis_{ptf.axis_},
  axialVelocity_{ptf.axialVelocity_().clone().ptr()},
  radialVelocity_{ptf.radialVelocity_().clone().ptr()},
  rpm_{ptf.rpm_().clone().ptr()}
{}

mousse::cylindricalInletVelocityFvPatchVectorField::
cylindricalInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>{p, iF, dict},
  centre_{dict.lookup("centre")},
  axis_{dict.lookup("axis")},
  axialVelocity_{DataEntry<scalar>::New("axialVelocity", dict)},
  radialVelocity_{DataEntry<scalar>::New("radialVelocity", dict)},
  rpm_{DataEntry<scalar>::New("rpm", dict)}
{}

mousse::cylindricalInletVelocityFvPatchVectorField::
cylindricalInletVelocityFvPatchVectorField
(
  const cylindricalInletVelocityFvPatchVectorField& ptf
)
:
  fixedValueFvPatchField<vector>(ptf),
  centre_(ptf.centre_),
  axis_(ptf.axis_),
  axialVelocity_(ptf.axialVelocity_().clone().ptr()),
  radialVelocity_(ptf.radialVelocity_().clone().ptr()),
  rpm_(ptf.rpm_().clone().ptr())
{}
mousse::cylindricalInletVelocityFvPatchVectorField::
cylindricalInletVelocityFvPatchVectorField
(
  const cylindricalInletVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>(ptf, iF),
  centre_(ptf.centre_),
  axis_(ptf.axis_),
  axialVelocity_(ptf.axialVelocity_().clone().ptr()),
  radialVelocity_(ptf.radialVelocity_().clone().ptr()),
  rpm_(ptf.rpm_().clone().ptr())
{}

// Member Functions
void mousse::cylindricalInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  const scalar axialVelocity = axialVelocity_->value(t);
  const scalar radialVelocity = radialVelocity_->value(t);
  const scalar rpm = rpm_->value(t);
  vector hatAxis = axis_/mag(axis_);
  const vectorField r(patch().Cf() - centre_);
  const vectorField d(r - (hatAxis & r)*hatAxis);
  tmp<vectorField> tangVel
  (
    (rpm*constant::mathematical::pi/30.0)*(hatAxis) ^ d
  );
  operator==(tangVel + hatAxis*axialVelocity + radialVelocity*d/mag(d));
  fixedValueFvPatchField<vector>::updateCoeffs();
}
void mousse::cylindricalInletVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchField<vector>::write(os);
  os.writeKeyword("centre") << centre_ << token::END_STATEMENT << nl;
  os.writeKeyword("axis") << axis_ << token::END_STATEMENT << nl;
  axialVelocity_->writeData(os);
  radialVelocity_->writeData(os);
  rpm_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
 fvPatchVectorField,
 cylindricalInletVelocityFvPatchVectorField
);

}

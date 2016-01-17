// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotating_pressure_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"

// Private Member Functions 
void mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
calcTangentialVelocity()
{
  const scalar t = this->db().time().timeOutputValue();
  vector om = omega_->value(t);
  vector axisHat = om/mag(om);
  const vectorField tangentialVelocity
  (
    (-om) ^ (patch().Cf() - axisHat*(axisHat & patch().Cf()))
  );
  const vectorField n(patch().nf());
  refValue() = tangentialVelocity - n*(n & tangentialVelocity);
}
// Constructors 
mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
rotatingPressureInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  pressureInletOutletVelocityFvPatchVectorField(p, iF),
  omega_()
{}
mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
rotatingPressureInletOutletVelocityFvPatchVectorField
(
  const rotatingPressureInletOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  pressureInletOutletVelocityFvPatchVectorField(ptf, p, iF, mapper),
  omega_(ptf.omega_().clone().ptr())
{
  calcTangentialVelocity();
}
mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
rotatingPressureInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  pressureInletOutletVelocityFvPatchVectorField(p, iF, dict),
  omega_(DataEntry<vector>::New("omega", dict))
{
  calcTangentialVelocity();
}
mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
rotatingPressureInletOutletVelocityFvPatchVectorField
(
  const rotatingPressureInletOutletVelocityFvPatchVectorField& rppvf
)
:
  pressureInletOutletVelocityFvPatchVectorField(rppvf),
  omega_(rppvf.omega_().clone().ptr())
{
  calcTangentialVelocity();
}
mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::
rotatingPressureInletOutletVelocityFvPatchVectorField
(
  const rotatingPressureInletOutletVelocityFvPatchVectorField& rppvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  pressureInletOutletVelocityFvPatchVectorField(rppvf, iF),
  omega_(rppvf.omega_().clone().ptr())
{
  calcTangentialVelocity();
}
// Member Functions 
void mousse::rotatingPressureInletOutletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("phi") << phiName() << token::END_STATEMENT << nl;
  omega_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  rotatingPressureInletOutletVelocityFvPatchVectorField
);

}

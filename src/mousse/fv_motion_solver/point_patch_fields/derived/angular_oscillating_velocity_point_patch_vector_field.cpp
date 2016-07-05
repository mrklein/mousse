// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "angular_oscillating_velocity_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"


namespace mousse {

// Constructors 
angularOscillatingVelocityPointPatchVectorField::
angularOscillatingVelocityPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{p, iF},
  axis_{vector::zero},
  origin_{vector::zero},
  angle0_{0.0},
  amplitude_{0.0},
  omega_{0.0},
  p0_{p.localPoints()}
{}


angularOscillatingVelocityPointPatchVectorField::
angularOscillatingVelocityPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>{p, iF, dict},
  axis_{dict.lookup("axis")},
  origin_{dict.lookup("origin")},
  angle0_{readScalar(dict.lookup("angle0"))},
  amplitude_{readScalar(dict.lookup("amplitude"))},
  omega_{readScalar(dict.lookup("omega"))}
{
  if (!dict.found("value")) {
    updateCoeffs();
  }
  if (dict.found("p0")) {
    p0_ = vectorField("p0", dict , p.size());
  } else {
    p0_ = p.localPoints();
  }
}


angularOscillatingVelocityPointPatchVectorField::
angularOscillatingVelocityPointPatchVectorField
(
  const angularOscillatingVelocityPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>{ptf, p, iF, mapper},
  axis_{ptf.axis_},
  origin_{ptf.origin_},
  angle0_{ptf.angle0_},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  p0_{ptf.p0_}
{}


angularOscillatingVelocityPointPatchVectorField::
angularOscillatingVelocityPointPatchVectorField
(
  const angularOscillatingVelocityPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{ptf, iF},
  axis_{ptf.axis_},
  origin_{ptf.origin_},
  angle0_{ptf.angle0_},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  p0_{ptf.p0_}
{}


// Member Functions 
void angularOscillatingVelocityPointPatchVectorField::autoMap
(
  const pointPatchFieldMapper& m
)
{
  fixedValuePointPatchField<vector>::autoMap(m);
  p0_.autoMap(m);
}


void angularOscillatingVelocityPointPatchVectorField::rmap
(
  const pointPatchField<vector>& ptf,
  const labelList& addr
)
{
  const angularOscillatingVelocityPointPatchVectorField& aOVptf =
    refCast<const angularOscillatingVelocityPointPatchVectorField>(ptf);
  fixedValuePointPatchField<vector>::rmap(aOVptf, addr);
  p0_.rmap(aOVptf.p0_, addr);
}


void angularOscillatingVelocityPointPatchVectorField::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  const pointPatch& p = this->patch();
  scalar angle = angle0_ + amplitude_*sin(omega_*t.value());
  vector axisHat = axis_/mag(axis_);
  vectorField p0Rel(p0_ - origin_);
  vectorField::operator=
  (
    (
      p0_
      + p0Rel*(cos(angle) - 1)
      + (axisHat ^ p0Rel*sin(angle))
      + (axisHat & p0Rel)*(1 - cos(angle))*axisHat
      - p.localPoints()
    )/t.deltaTValue()
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}


void angularOscillatingVelocityPointPatchVectorField::write
(
  Ostream& os
) const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("axis") << axis_ << token::END_STATEMENT << nl;
  os.writeKeyword("origin") << origin_ << token::END_STATEMENT << nl;
  os.writeKeyword("angle0") << angle0_ << token::END_STATEMENT << nl;
  os.writeKeyword("amplitude") << amplitude_ << token::END_STATEMENT << nl;
  os.writeKeyword("omega") << omega_ << token::END_STATEMENT << nl;
  p0_.writeEntry("p0", os);
  writeEntry("value", os);
}


MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  angularOscillatingVelocityPointPatchVectorField
);

}  // namespace mousse


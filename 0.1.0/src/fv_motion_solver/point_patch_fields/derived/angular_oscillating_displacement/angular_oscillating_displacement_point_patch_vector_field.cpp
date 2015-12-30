// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "angular_oscillating_displacement_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
// Constructors 
angularOscillatingDisplacementPointPatchVectorField::
angularOscillatingDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>(p, iF),
  axis_(vector::zero),
  origin_(vector::zero),
  angle0_(0.0),
  amplitude_(0.0),
  omega_(0.0),
  p0_(p.localPoints())
{}
angularOscillatingDisplacementPointPatchVectorField::
angularOscillatingDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>(p, iF, dict),
  axis_(dict.lookup("axis")),
  origin_(dict.lookup("origin")),
  angle0_(readScalar(dict.lookup("angle0"))),
  amplitude_(readScalar(dict.lookup("amplitude"))),
  omega_(readScalar(dict.lookup("omega")))
{
  if (!dict.found("value"))
  {
    updateCoeffs();
  }
  if (dict.found("p0"))
  {
    p0_ = vectorField("p0", dict , p.size());
  }
  else
  {
    p0_ = p.localPoints();
  }
}
angularOscillatingDisplacementPointPatchVectorField::
angularOscillatingDisplacementPointPatchVectorField
(
  const angularOscillatingDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>(ptf, p, iF, mapper),
  axis_(ptf.axis_),
  origin_(ptf.origin_),
  angle0_(ptf.angle0_),
  amplitude_(ptf.amplitude_),
  omega_(ptf.omega_),
  p0_(ptf.p0_, mapper)
{}
angularOscillatingDisplacementPointPatchVectorField::
angularOscillatingDisplacementPointPatchVectorField
(
  const angularOscillatingDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>(ptf, iF),
  axis_(ptf.axis_),
  origin_(ptf.origin_),
  angle0_(ptf.angle0_),
  amplitude_(ptf.amplitude_),
  omega_(ptf.omega_),
  p0_(ptf.p0_)
{}
// Member Functions 
void angularOscillatingDisplacementPointPatchVectorField::autoMap
(
  const pointPatchFieldMapper& m
)
{
  fixedValuePointPatchField<vector>::autoMap(m);
  p0_.autoMap(m);
}
void angularOscillatingDisplacementPointPatchVectorField::rmap
(
  const pointPatchField<vector>& ptf,
  const labelList& addr
)
{
  const angularOscillatingDisplacementPointPatchVectorField& aODptf =
    refCast<const angularOscillatingDisplacementPointPatchVectorField>(ptf);
  fixedValuePointPatchField<vector>::rmap(aODptf, addr);
  p0_.rmap(aODptf.p0_, addr);
}
void angularOscillatingDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  scalar angle = angle0_ + amplitude_*sin(omega_*t.value());
  vector axisHat = axis_/mag(axis_);
  vectorField p0Rel(p0_ - origin_);
  vectorField::operator=
  (
    p0Rel*(cos(angle) - 1)
   + (axisHat ^ p0Rel*sin(angle))
   + (axisHat & p0Rel)*(1 - cos(angle))*axisHat
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}
void angularOscillatingDisplacementPointPatchVectorField::write
(
  Ostream& os
) const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("axis")
    << axis_ << token::END_STATEMENT << nl;
  os.writeKeyword("origin")
    << origin_ << token::END_STATEMENT << nl;
  os.writeKeyword("angle0")
    << angle0_ << token::END_STATEMENT << nl;
  os.writeKeyword("amplitude")
    << amplitude_ << token::END_STATEMENT << nl;
  os.writeKeyword("omega")
    << omega_ << token::END_STATEMENT << nl;
  p0_.writeEntry("p0", os);
  writeEntry("value", os);
}
makePointPatchTypeField
(
  pointPatchVectorField,
  angularOscillatingDisplacementPointPatchVectorField
);
}  // namespace mousse

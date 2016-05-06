// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "oscillating_velocity_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"


namespace mousse {

// Constructors 
oscillatingVelocityPointPatchVectorField::
oscillatingVelocityPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{p, iF},
  amplitude_{vector::zero},
  omega_{0.0},
  p0_{p.localPoints()}
{}


oscillatingVelocityPointPatchVectorField::
oscillatingVelocityPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>{p, iF, dict},
  amplitude_{dict.lookup("amplitude")},
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


oscillatingVelocityPointPatchVectorField::
oscillatingVelocityPointPatchVectorField
(
  const oscillatingVelocityPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>{ptf, p, iF, mapper},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  p0_{ptf.p0_, mapper}
{}


oscillatingVelocityPointPatchVectorField::
oscillatingVelocityPointPatchVectorField
(
  const oscillatingVelocityPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{ptf, iF},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  p0_{ptf.p0_}
{}


// Member Functions 
void oscillatingVelocityPointPatchVectorField::autoMap
(
  const pointPatchFieldMapper& m
)
{
  fixedValuePointPatchField<vector>::autoMap(m);
  p0_.autoMap(m);
}


void oscillatingVelocityPointPatchVectorField::rmap
(
  const pointPatchField<vector>& ptf,
  const labelList& addr
)
{
  const oscillatingVelocityPointPatchVectorField& oVptf =
    refCast<const oscillatingVelocityPointPatchVectorField>(ptf);
  fixedValuePointPatchField<vector>::rmap(oVptf, addr);
  p0_.rmap(oVptf.p0_, addr);
}


void oscillatingVelocityPointPatchVectorField::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  const pointPatch& p = this->patch();
  Field<vector>::operator=
  (
    (p0_ + amplitude_*sin(omega_*t.value()) - p.localPoints())/t.deltaTValue()
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}


void oscillatingVelocityPointPatchVectorField::write(Ostream& os) const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("amplitude") << amplitude_ << token::END_STATEMENT << nl;
  os.writeKeyword("omega") << omega_ << token::END_STATEMENT << nl;
  p0_.writeEntry("p0", os);
  writeEntry("value", os);
}


MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  oscillatingVelocityPointPatchVectorField
);

}  // namespace mousse


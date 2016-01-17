// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "oscillating_displacement_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
// Constructors 
oscillatingDisplacementPointPatchVectorField::
oscillatingDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{p, iF},
  amplitude_{vector::zero},
  omega_{0.0}
{}
oscillatingDisplacementPointPatchVectorField::
oscillatingDisplacementPointPatchVectorField
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
  if (!dict.found("value"))
  {
    updateCoeffs();
  }
}
oscillatingDisplacementPointPatchVectorField::
oscillatingDisplacementPointPatchVectorField
(
  const oscillatingDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>{ptf, p, iF, mapper},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_}
{}
oscillatingDisplacementPointPatchVectorField::
oscillatingDisplacementPointPatchVectorField
(
  const oscillatingDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{ptf, iF},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_}
{}
// Member Functions 
void oscillatingDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  Field<vector>::operator=(amplitude_*sin(omega_*t.value()));
  fixedValuePointPatchField<vector>::updateCoeffs();
}
void oscillatingDisplacementPointPatchVectorField::write(Ostream& os) const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("amplitude") << amplitude_ << token::END_STATEMENT << nl;
  os.writeKeyword("omega") << omega_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  oscillatingDisplacementPointPatchVectorField
);
}  // namespace mousse

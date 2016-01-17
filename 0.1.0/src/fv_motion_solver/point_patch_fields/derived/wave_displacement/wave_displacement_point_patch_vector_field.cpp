// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wave_displacement_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
// Constructors 
mousse::waveDisplacementPointPatchVectorField::
waveDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{p, iF},
  amplitude_{vector::zero},
  omega_{0.0},
  waveNumber_{vector::zero}
{}
mousse::waveDisplacementPointPatchVectorField::
waveDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>{p, iF, dict},
  amplitude_{dict.lookup("amplitude")},
  omega_{readScalar(dict.lookup("omega"))},
  waveNumber_{dict.lookupOrDefault<vector>("waveNumber", vector::zero)}
{
  if (!dict.found("value"))
  {
    updateCoeffs();
  }
}
mousse::waveDisplacementPointPatchVectorField::
waveDisplacementPointPatchVectorField
(
  const waveDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>{ptf, p, iF, mapper},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  waveNumber_{ptf.waveNumber_}
{}
mousse::waveDisplacementPointPatchVectorField::
waveDisplacementPointPatchVectorField
(
  const waveDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{ptf, iF},
  amplitude_{ptf.amplitude_},
  omega_{ptf.omega_},
  waveNumber_{ptf.waveNumber_}
{}
// Member Functions 
void mousse::waveDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  const scalarField points( waveNumber_ & patch().localPoints());
  Field<vector>::operator=
  (
    amplitude_*cos(omega_*t.value() - points)
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}
void mousse::waveDisplacementPointPatchVectorField::write(Ostream& os) const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("amplitude")
    << amplitude_ << token::END_STATEMENT << nl;
  os.writeKeyword("omega")
    << omega_ << token::END_STATEMENT << nl;
  os.writeKeyword("waveNumber")
    << waveNumber_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  waveDisplacementPointPatchVectorField
);
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "translating_wall_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"

// Constructors 
mousse::translatingWallVelocityFvPatchVectorField::
translatingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{p, iF},
  U_{vector::zero}
{}

mousse::translatingWallVelocityFvPatchVectorField::
translatingWallVelocityFvPatchVectorField
(
  const translatingWallVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>{ptf, p, iF, mapper},
  U_{ptf.U_}
{}

mousse::translatingWallVelocityFvPatchVectorField::
translatingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>{p, iF},
  U_{dict.lookup("U")}
{
  // Evaluate the wall velocity
  updateCoeffs();
}

mousse::translatingWallVelocityFvPatchVectorField::
translatingWallVelocityFvPatchVectorField
(
  const translatingWallVelocityFvPatchVectorField& twvpvf
)
:
  fixedValueFvPatchField<vector>{twvpvf},
  U_{twvpvf.U_}
{}

mousse::translatingWallVelocityFvPatchVectorField::
translatingWallVelocityFvPatchVectorField
(
  const translatingWallVelocityFvPatchVectorField& twvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{twvpvf, iF},
  U_{twvpvf.U_}
{}

// Member Functions 
void mousse::translatingWallVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Remove the component of U normal to the wall in case the wall is not flat
  const vectorField n(patch().nf());
  vectorField::operator=(U_ - n*(n & U_));
  fixedValueFvPatchVectorField::updateCoeffs();
}

void mousse::translatingWallVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("U") << U_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}

namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  translatingWallVelocityFvPatchVectorField
);
}

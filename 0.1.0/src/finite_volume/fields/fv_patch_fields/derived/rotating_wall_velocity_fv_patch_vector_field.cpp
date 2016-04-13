// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotating_wall_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"


// Constructors 
mousse::rotatingWallVelocityFvPatchVectorField::
rotatingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{p, iF},
  origin_{},
  axis_{vector::zero},
  omega_{0}
{}


mousse::rotatingWallVelocityFvPatchVectorField::
rotatingWallVelocityFvPatchVectorField
(
  const rotatingWallVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>{ptf, p, iF, mapper},
  origin_{ptf.origin_},
  axis_{ptf.axis_},
  omega_{ptf.omega_().clone().ptr()}
{}


mousse::rotatingWallVelocityFvPatchVectorField::
rotatingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>{p, iF},
  origin_{dict.lookup("origin")},
  axis_{dict.lookup("axis")},
  omega_{DataEntry<scalar>::New("omega", dict)}
{
  if (dict.found("value")) {
    fvPatchField<vector>::operator=
    (
      vectorField{"value", dict, p.size()}
    );
  } else {
    // Evaluate the wall velocity
    updateCoeffs();
  }
}


mousse::rotatingWallVelocityFvPatchVectorField::
rotatingWallVelocityFvPatchVectorField
(
  const rotatingWallVelocityFvPatchVectorField& rwvpvf
)
:
  fixedValueFvPatchField<vector>{rwvpvf},
  origin_{rwvpvf.origin_},
  axis_{rwvpvf.axis_},
  omega_{rwvpvf.omega_().clone().ptr()}
{}


mousse::rotatingWallVelocityFvPatchVectorField::
rotatingWallVelocityFvPatchVectorField
(
  const rotatingWallVelocityFvPatchVectorField& rwvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{rwvpvf, iF},
  origin_{rwvpvf.origin_},
  axis_{rwvpvf.axis_},
  omega_{rwvpvf.omega_().clone().ptr()}
{}


// Member Functions 
void mousse::rotatingWallVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  scalar om = omega_->value(t);
  // Calculate the rotating wall velocity from the specification of the motion
  const vectorField Up
  {
    (-om)*((patch().Cf() - origin_) ^ (axis_/mag(axis_)))
  };
  // Remove the component of Up normal to the wall
  // just in case it is not exactly circular
  const vectorField n{patch().nf()};
  vectorField::operator=(Up - n*(n & Up));
  fixedValueFvPatchVectorField::updateCoeffs();
}


void mousse::rotatingWallVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("origin") << origin_ << token::END_STATEMENT << nl;
  os.writeKeyword("axis") << axis_ << token::END_STATEMENT << nl;
  omega_->writeData(os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  rotatingWallVelocityFvPatchVectorField
);

}


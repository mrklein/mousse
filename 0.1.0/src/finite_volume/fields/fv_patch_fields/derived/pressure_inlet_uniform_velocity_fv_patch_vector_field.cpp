// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_inlet_uniform_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::pressureInletUniformVelocityFvPatchVectorField::
pressureInletUniformVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  pressureInletVelocityFvPatchVectorField(p, iF)
{}
mousse::pressureInletUniformVelocityFvPatchVectorField::
pressureInletUniformVelocityFvPatchVectorField
(
  const pressureInletUniformVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  pressureInletVelocityFvPatchVectorField(ptf, p, iF, mapper)
{}
mousse::pressureInletUniformVelocityFvPatchVectorField::
pressureInletUniformVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  pressureInletVelocityFvPatchVectorField(p, iF, dict)
{}
mousse::pressureInletUniformVelocityFvPatchVectorField::
pressureInletUniformVelocityFvPatchVectorField
(
  const pressureInletUniformVelocityFvPatchVectorField& pivpvf
)
:
  pressureInletVelocityFvPatchVectorField(pivpvf)
{}
mousse::pressureInletUniformVelocityFvPatchVectorField::
pressureInletUniformVelocityFvPatchVectorField
(
  const pressureInletUniformVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  pressureInletVelocityFvPatchVectorField(pivpvf, iF)
{}
// Member Functions 
void mousse::pressureInletUniformVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  pressureInletVelocityFvPatchVectorField::updateCoeffs();
  operator==(patch().nf()*gSum(patch().Sf() & *this)/gSum(patch().magSf()));
}
// Member Operators 
void mousse::pressureInletUniformVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  operator==(patch().nf()*gSum(patch().Sf() & pvf)/gSum(patch().magSf()));
}
namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureInletUniformVelocityFvPatchVectorField
);

}

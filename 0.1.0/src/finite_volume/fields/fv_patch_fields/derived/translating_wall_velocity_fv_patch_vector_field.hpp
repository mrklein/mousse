#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_TRANSLATING_WALL_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_TRANSLATING_WALL_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::translatingWallVelocityFvPatchVectorField
// Group
//   grpWallBoundaryConditions grpGenericBoundaryConditions
// Description
//   This boundary condition provides a velocity condition for translational
//   motion on walls.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | translational velocity  | yes|
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            translatingWallVelocity;
//     U               (100 0 0);
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   translating_wall_velocity_fv_patch_vector_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class translatingWallVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Translational velocity
    vector U_;
public:
  //- Runtime type information
  TYPE_NAME("translatingWallVelocity");
  // Constructors
    //- Construct from patch and internal field
    translatingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    translatingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given a
    // translatingWallVelocityFvPatchVectorField onto a new patch
    translatingWallVelocityFvPatchVectorField
    (
      const translatingWallVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    translatingWallVelocityFvPatchVectorField
    (
      const translatingWallVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new translatingWallVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    translatingWallVelocityFvPatchVectorField
    (
      const translatingWallVelocityFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new translatingWallVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access functions
      //- Return the velocity
      const vector& U() const
      {
        return U_;
      }
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif

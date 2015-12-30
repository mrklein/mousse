// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::movingWallVelocityFvPatchVectorField
// Group
//   grpWallBoundaryConditions
// Description
//   This boundary condition provides a velocity condition for cases with
//   moving walls.  In addition, it should also be applied to 'moving' walls
//   for moving reference frame (MRF) calculations.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velociy field name      | no          | U
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            movingWallVelocity;
//     U               U;
//     value           uniform 0; // initial value
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchVectorField
//   mousse::MRFZone
// SourceFiles
//   moving_wall_velocity_fv_patch_vector_field.cpp
#ifndef moving_wall_velocity_fv_patch_vector_field_hpp_
#define moving_wall_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class movingWallVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
   //- Name of velocity field
   word UName_;
public:
  //- Runtime type information
  TypeName("movingWallVelocity");
  // Constructors
    //- Construct from patch and internal field
    movingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    movingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given movingWallVelocityFvPatchVectorField
    //  onto a new patch
    movingWallVelocityFvPatchVectorField
    (
      const movingWallVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    movingWallVelocityFvPatchVectorField
    (
      const movingWallVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new movingWallVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    movingWallVelocityFvPatchVectorField
    (
      const movingWallVelocityFvPatchVectorField&,
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
        new movingWallVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif

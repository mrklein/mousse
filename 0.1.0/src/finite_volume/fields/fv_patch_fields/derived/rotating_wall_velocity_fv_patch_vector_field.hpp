// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rotatingWallVelocityFvPatchVectorField
// Group
//   grpWallBoundaryConditions grpGenericBoundaryConditions
// Description
//   This boundary condition provides a rotational velocity condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     origin       | origin of rotation in Cartesian co-ordinates | yes|
//     axis         | axis of rotation        | yes         |
//     omega        | angular velocty of the frame [rad/s] | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            rotatingWallVelocity;
//     origin          (0 0 0);
//     axis            (0 0 1);
//     omega           100;
//   }
//   \endverbatim
//   The \c omega entry is a DataEntry type, able to describe time varying
//   functions.
// SeeAlso
//   mousse::DataEntry
//   mousse::fixedValueFvPatchField
// SourceFiles
//   rotating_wall_velocity_fv_patch_vector_field.cpp
#ifndef rotating_wall_velocity_fv_patch_vector_field_hpp_
#define rotating_wall_velocity_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class rotatingWallVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Origin of the rotation
    vector origin_;
    //- Axis of the rotation
    vector axis_;
    //- Rotational speed
    autoPtr<DataEntry<scalar> > omega_;
public:
  //- Runtime type information
  TYPE_NAME("rotatingWallVelocity");
  // Constructors
    //- Construct from patch and internal field
    rotatingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    rotatingWallVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given rotatingWallVelocityFvPatchVectorField
    //  onto a new patch
    rotatingWallVelocityFvPatchVectorField
    (
      const rotatingWallVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    rotatingWallVelocityFvPatchVectorField
    (
      const rotatingWallVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new rotatingWallVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    rotatingWallVelocityFvPatchVectorField
    (
      const rotatingWallVelocityFvPatchVectorField&,
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
        new rotatingWallVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access functions
      //- Return the origin of the rotation
      const vector& origin() const
      {
        return origin_;
      }
      //- Return the axis of the rotation
      const vector& axis() const
      {
        return axis_;
      }
      //- Return non-const access to the origin of the rotation
      vector& origin()
      {
        return origin_;
      }
      //- Return non-const access to the axis of the rotation
      vector& axis()
      {
        return axis_;
      }
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif

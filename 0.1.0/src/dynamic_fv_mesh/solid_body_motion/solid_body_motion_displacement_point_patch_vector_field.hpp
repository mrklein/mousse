#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_POINT_PATCH_FIELDS_DERIVED_SOLID_BODY_MOTION_DISPLACEMENT_SOLID_BODY_MOTION_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_POINT_PATCH_FIELDS_DERIVED_SOLID_BODY_MOTION_DISPLACEMENT_SOLID_BODY_MOTION_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionDisplacementPointPatchVectorField
// Description
//   Enables the specification of a fixed value boundary condition using the
//   solid body motion functions.

#include "solid_body_motion_function.hpp"
#include "fixed_value_point_patch_fields.hpp"


namespace mousse {

class solidBodyMotionDisplacementPointPatchVectorField
:
  public fixedValuePointPatchVectorField
{
  // Private data
    //- The motion control function
    autoPtr<solidBodyMotionFunction> SBMFPtr_;
    mutable autoPtr<pointField> localPoints0Ptr_;
public:
  //- Runtime type information
  TYPE_NAME("solidBodyMotionDisplacement");
  // Constructors
    //- Construct from patch and internal field
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const solidBodyMotionDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const solidBodyMotionDisplacementPointPatchVectorField&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector>> clone() const
    {
      return autoPtr<pointPatchField<vector>>
      {
        new solidBodyMotionDisplacementPointPatchVectorField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const solidBodyMotionDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<vector>> clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<vector>>
      {
        new solidBodyMotionDisplacementPointPatchVectorField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the fluctuation scale
      const solidBodyMotionFunction& motion() const
      {
        return SBMFPtr_();
      }
      const pointField& localPoints0() const;
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

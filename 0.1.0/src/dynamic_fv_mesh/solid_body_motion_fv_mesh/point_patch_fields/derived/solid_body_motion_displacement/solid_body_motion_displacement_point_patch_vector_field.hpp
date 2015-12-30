// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionDisplacementPointPatchVectorField
// Description
//   Enables the specification of a fixed value boundary condition using the
//   solid body motion functions.
// SourceFiles
//   solid_body_motion_displacement_point_patch_vector_field.cpp
#ifndef solid_body_motion_displacement_point_patch_vector_field_hpp_
#define solid_body_motion_displacement_point_patch_vector_field_hpp_
#include "solid_body_motion_function.hpp"
#include "fixed_value_point_patch_fields.hpp"
namespace mousse
{
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
  TypeName("solidBodyMotionDisplacement");
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
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new solidBodyMotionDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    solidBodyMotionDisplacementPointPatchVectorField
    (
      const solidBodyMotionDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<vector> > clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new solidBodyMotionDisplacementPointPatchVectorField
        (
          *this,
          iF
        )
      );
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

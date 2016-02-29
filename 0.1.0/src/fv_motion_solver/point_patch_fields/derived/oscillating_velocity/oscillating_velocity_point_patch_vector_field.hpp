#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_OSCILLATING_VELOCITY_OSCILLATING_VELOCITY_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_OSCILLATING_VELOCITY_OSCILLATING_VELOCITY_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::oscillatingVelocityPointPatchVectorField
// Description
//   mousse::oscillatingVelocityPointPatchVectorField
// SourceFiles
//   oscillating_velocity_point_patch_vector_field.cpp
#include "fixed_value_point_patch_field.hpp"
namespace mousse
{
class oscillatingVelocityPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    vector amplitude_;
    scalar omega_;
    pointField p0_;
public:
  //- Runtime type information
  TYPE_NAME("oscillatingVelocity");
  // Constructors
    //- Construct from patch and internal field
    oscillatingVelocityPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    oscillatingVelocityPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    oscillatingVelocityPointPatchVectorField
    (
      const oscillatingVelocityPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new oscillatingVelocityPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    oscillatingVelocityPointPatchVectorField
    (
      const oscillatingVelocityPointPatchVectorField&,
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
        new oscillatingVelocityPointPatchVectorField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const pointPatchFieldMapper&
      );
      //- Reverse map the given pointPatchField onto this pointPatchField
      virtual void rmap
      (
        const pointPatchField<vector>&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif

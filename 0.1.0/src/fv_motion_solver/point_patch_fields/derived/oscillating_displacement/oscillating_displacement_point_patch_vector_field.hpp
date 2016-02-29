#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_OSCILLATING_DISPLACEMENT_OSCILLATING_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_OSCILLATING_DISPLACEMENT_OSCILLATING_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::oscillatingDisplacementPointPatchVectorField
// Description
//   mousse::oscillatingDisplacementPointPatchVectorField
// SourceFiles
//   oscillating_displacement_point_patch_vector_field.cpp
#include "fixed_value_point_patch_field.hpp"
namespace mousse
{
class oscillatingDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    vector amplitude_;
    scalar omega_;
public:
  //- Runtime type information
  TYPE_NAME("oscillatingDisplacement");
  // Constructors
    //- Construct from patch and internal field
    oscillatingDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    oscillatingDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    oscillatingDisplacementPointPatchVectorField
    (
      const oscillatingDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new oscillatingDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    oscillatingDisplacementPointPatchVectorField
    (
      const oscillatingDisplacementPointPatchVectorField&,
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
        new oscillatingDisplacementPointPatchVectorField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif

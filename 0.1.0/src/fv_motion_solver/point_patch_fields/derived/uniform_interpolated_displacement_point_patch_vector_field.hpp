#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_UNIFORM_INTERPOLATED_DISPLACEMENT_UNIFORM_INTERPOLATED_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_UNIFORM_INTERPOLATED_DISPLACEMENT_UNIFORM_INTERPOLATED_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformInterpolatedDisplacementPointPatchVectorField
// Description
//   Interpolates pre-specified motion.
//   Motion specified as pointVectorFields. E.g.
//   walls
//   {
//     type                uniformInterpolatedDisplacement;
//     value               uniform (0 0 0);
//     fieldName           wantedDisplacement;
//     interpolationScheme linear;
//   }
//   This will scan the case for 'wantedDisplacement' pointVectorFields
//   and interpolate those in time (using 'linear' interpolation) to
//   obtain the current displacement.
//   The advantage of specifying displacement in this way is that it
//   automatically works through decomposePar.

#include "fixed_value_point_patch_field.hpp"


namespace mousse{

class interpolationWeights;


class uniformInterpolatedDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    //- Name of displacement field
    const word fieldName_;
    const word interpolationScheme_;
    //- Times with pre-specified displacement
    wordList timeNames_;
    //- Times with pre-specified displacement
    scalarField timeVals_;
    //- User-specified interpolator
    autoPtr<interpolationWeights> interpolatorPtr_;
    //- Cached interpolation times
    labelList currentIndices_;
    //- Cached interpolation weights
    scalarField currentWeights_;
public:
  //- Runtime type information
  TYPE_NAME("uniformInterpolatedDisplacement");
  // Constructors
    //- Construct from patch and internal field
    uniformInterpolatedDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformInterpolatedDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    uniformInterpolatedDisplacementPointPatchVectorField
    (
      const uniformInterpolatedDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector>> clone() const
    {
      return
        autoPtr<pointPatchField<vector>>
        {
          new uniformInterpolatedDisplacementPointPatchVectorField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    uniformInterpolatedDisplacementPointPatchVectorField
    (
      const uniformInterpolatedDisplacementPointPatchVectorField&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<vector>> clone
    (
      const DimensionedField<vector, pointMesh>& iF
    ) const
    {
      return
        autoPtr<pointPatchField<vector>>
        {
          new uniformInterpolatedDisplacementPointPatchVectorField{*this, iF}
        };
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


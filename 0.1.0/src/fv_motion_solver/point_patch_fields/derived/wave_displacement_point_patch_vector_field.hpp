#ifndef FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_WAVE_DISPLACEMENT_WAVE_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_
#define FV_MOTION_SOLVER_POINT_PATCH_FIELDS_DERIVED_WAVE_DISPLACEMENT_WAVE_DISPLACEMENT_POINT_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::waveDisplacementPointPatchVectorField
// Description
//   mousse::waveDisplacementPointPatchVectorField

#include "fixed_value_point_patch_field.hpp"


namespace mousse {

class waveDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    vector amplitude_;
    scalar omega_;
    vector waveNumber_;
public:
  //- Runtime type information
  TYPE_NAME("waveDisplacement");
  // Constructors
    //- Construct from patch and internal field
    waveDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    waveDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    waveDisplacementPointPatchVectorField
    (
      const waveDisplacementPointPatchVectorField&,
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
          new waveDisplacementPointPatchVectorField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    waveDisplacementPointPatchVectorField
    (
      const waveDisplacementPointPatchVectorField&,
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
          new waveDisplacementPointPatchVectorField{*this, iF}
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


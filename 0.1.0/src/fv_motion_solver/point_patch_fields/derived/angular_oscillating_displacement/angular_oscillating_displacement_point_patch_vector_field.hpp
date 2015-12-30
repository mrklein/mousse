// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::angularOscillatingDisplacementPointPatchVectorField
// Description
//   mousse::angularOscillatingDisplacementPointPatchVectorField
// SourceFiles
//   angular_oscillating_displacement_point_patch_vector_field.cpp
#ifndef angular_oscillating_displacement_point_patch_vector_field_hpp_
#define angular_oscillating_displacement_point_patch_vector_field_hpp_
#include "fixed_value_point_patch_field.hpp"
namespace mousse
{
class angularOscillatingDisplacementPointPatchVectorField
:
  public fixedValuePointPatchField<vector>
{
  // Private data
    vector axis_;
    vector origin_;
    scalar angle0_;
    scalar amplitude_;
    scalar omega_;
    pointField p0_;
public:
  //- Runtime type information
  TypeName("angularOscillatingDisplacement");
  // Constructors
    //- Construct from patch and internal field
    angularOscillatingDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    angularOscillatingDisplacementPointPatchVectorField
    (
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<vector> onto a new patch
    angularOscillatingDisplacementPointPatchVectorField
    (
      const angularOscillatingDisplacementPointPatchVectorField&,
      const pointPatch&,
      const DimensionedField<vector, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<vector> > clone() const
    {
      return autoPtr<pointPatchField<vector> >
      (
        new angularOscillatingDisplacementPointPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    angularOscillatingDisplacementPointPatchVectorField
    (
      const angularOscillatingDisplacementPointPatchVectorField&,
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
        new angularOscillatingDisplacementPointPatchVectorField
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

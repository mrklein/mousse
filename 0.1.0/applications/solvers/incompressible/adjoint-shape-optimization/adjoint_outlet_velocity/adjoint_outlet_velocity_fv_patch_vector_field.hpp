// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adjointOutletVelocityFvPatchVectorField
// Description
// SourceFiles
//   adjoint_outlet_velocity_fv_patch_vector_field.cpp
#ifndef adjoint_outlet_velocity_fv_patch_vector_field_hpp_
#define adjoint_outlet_velocity_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class adjointOutletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
public:
  //- Runtime type information
  TYPE_NAME("adjointOutletVelocity");
  // Constructors
    //- Construct from patch and internal field
    adjointOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    adjointOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given adjointOutletVelocityFvPatchVectorField
    //  onto a new patch
    adjointOutletVelocityFvPatchVectorField
    (
      const adjointOutletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new adjointOutletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    adjointOutletVelocityFvPatchVectorField
    (
      const adjointOutletVelocityFvPatchVectorField&,
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
        new adjointOutletVelocityFvPatchVectorField(*this, iF)
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
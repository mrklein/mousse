#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_GRADIENT_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_GRADIENT_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformFixedGradientFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition provides a uniform fixed gradient condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     uniformGradient | uniform gradient     | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            uniformFixedGradient;
//     uniformGradient constant 0.2;
//   }
//   \endverbatim
// Note
//   The uniformGradient entry is a DataEntry type, able to describe time
//   varying functions.  The example above gives the usage for supplying a
//   constant value.
// SeeAlso
//   mousse::DataEntry
//   mousse::fixedGradientFvPatchField

#include "fixed_gradient_fv_patch_fields.hpp"
#include "data_entry.hpp"


namespace mousse {

template<class Type>
class uniformFixedGradientFvPatchField
:
  public fixedGradientFvPatchField<Type>
{
  // Private data
    //- Gradient
    autoPtr<DataEntry<Type>> uniformGradient_;
public:
  //- Runtime type information
  TYPE_NAME("uniformFixedGradient");
  // Constructors
    //- Construct from patch and internal field
    uniformFixedGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch and internal field and patch field
    uniformFixedGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>& fld
    );
    //- Construct from patch, internal field and dictionary
    uniformFixedGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given uniformFixedGradientFvPatchField
    //  onto a new patch
    uniformFixedGradientFvPatchField
    (
      const uniformFixedGradientFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformFixedGradientFvPatchField
    (
      const uniformFixedGradientFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new uniformFixedGradientFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    uniformFixedGradientFvPatchField
    (
      const uniformFixedGradientFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new uniformFixedGradientFvPatchField<Type>{*this, iF}
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

#include "uniform_fixed_gradient_fv_patch_field.ipp"

#endif

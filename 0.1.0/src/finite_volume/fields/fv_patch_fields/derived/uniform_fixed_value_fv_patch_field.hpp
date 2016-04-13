#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_VALUE_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_VALUE_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformFixedValueFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition provides a uniform fixed value condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     uniformValue | uniform value           |         yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            uniformFixedValue;
//     uniformValue    constant 0.2;
//   }
//   \endverbatim
// Note
//   The uniformValue entry is a DataEntry type, able to describe time
//   varying functions.  The example above gives the usage for supplying a
//   constant value.
// SeeAlso
//   mousse::DataEntry
//   mousse::fixedValueFvPatchField

#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"


namespace mousse {

template<class Type>
class uniformFixedValueFvPatchField
:
  public fixedValueFvPatchField<Type>
{
  // Private data
    autoPtr<DataEntry<Type> > uniformValue_;
public:
  //- Runtime type information
  TYPE_NAME("uniformFixedValue");
  // Constructors
    //- Construct from patch and internal field
    uniformFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch and internal field and patch field
    uniformFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>& fld
    );
    //- Construct from patch, internal field and dictionary
    uniformFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given uniformFixedValueFvPatchField
    //  onto a new patch
    uniformFixedValueFvPatchField
    (
      const uniformFixedValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformFixedValueFvPatchField
    (
      const uniformFixedValueFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new uniformFixedValueFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    uniformFixedValueFvPatchField
    (
      const uniformFixedValueFvPatchField<Type>&,
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
        new uniformFixedValueFvPatchField<Type>{*this, iF}
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

#include "uniform_fixed_value_fv_patch_field.ipp"

#endif

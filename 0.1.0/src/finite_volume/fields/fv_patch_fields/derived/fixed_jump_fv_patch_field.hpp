#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_JUMP_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_JUMP_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedJumpFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a jump condition, using the \c cyclic
//   condition as a base.
//   The jump is specified as a fixed value field, applied as an offset to the
//   'owner' patch.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     patchType    | underlying patch type should be \c cyclic| yes |
//     jump         | current jump value      | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedJump;
//     patchType       cyclic;
//     jump            uniform 10;
//   }
//   \endverbatim
//   The above example shows the use of a fixed jump of '10'.
// Note
//   The underlying \c patchType should be set to \c cyclic
// SeeAlso
//   mousse::jumpCyclicFvPatchField

#include "jump_cyclic_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class fixedJumpFvPatchField
:
  public jumpCyclicFvPatchField<Type>
{
protected:
  // Protected data
    //- "jump" field
    Field<Type> jump_;
public:
  //- Runtime type information
  TYPE_NAME("fixedJump");
  // Constructors
    //- Construct from patch and internal field
    fixedJumpFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedJumpFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedJumpFvPatchField onto a
    //  new patch
    fixedJumpFvPatchField
    (
      const fixedJumpFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedJumpFvPatchField
    (
      const fixedJumpFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new fixedJumpFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    fixedJumpFvPatchField
    (
      const fixedJumpFvPatchField<Type>&,
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
        new fixedJumpFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the "jump" across the patch
      virtual tmp<Field<Type>> jump() const;
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchField<Type>&,
        const labelList&
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "fixed_jump_fv_patch_field.ipp"

#endif

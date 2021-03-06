#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_SLIP_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_SLIP_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::slipFvPatchField
// Group
//   grpWallBoundaryConditions grpGenericBoundaryConditions
// Description
//   This boundary condition provides a slip constraint.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            slip;
//   }
//   \endverbatim

#include "basic_symmetry_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class slipFvPatchField
:
  public basicSymmetryFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("slip");
  // Constructors
    //- Construct from patch and internal field
    slipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    slipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given slipFvPatchField onto a new patch
    slipFvPatchField
    (
      const slipFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    slipFvPatchField
    (
      const slipFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new slipFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    slipFvPatchField
    (
      const slipFvPatchField<Type>&,
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
        new slipFvPatchField<Type>{*this, iF}
      };
    }
};
}  // namespace mousse

#include "slip_fv_patch_field.ipp"

#endif

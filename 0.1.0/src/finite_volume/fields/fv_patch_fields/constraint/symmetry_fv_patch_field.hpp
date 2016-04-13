#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_SYMMETRY_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_SYMMETRY_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryFvPatchField
// Group
//   grpConstraintBoundaryConditions
// Description
//   This boundary condition enforces a symmetry constraint
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            symmetry;
//   }
//   \endverbatim

#include "basic_symmetry_fv_patch_field.hpp"
#include "symmetry_fv_patch.hpp"


namespace mousse {

template<class Type>
class symmetryFvPatchField
:
  public basicSymmetryFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(symmetryFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    symmetryFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    symmetryFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given symmetryFvPatchField onto a new patch
    symmetryFvPatchField
    (
      const symmetryFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    symmetryFvPatchField
    (
      const symmetryFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new symmetryFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    symmetryFvPatchField
    (
      const symmetryFvPatchField<Type>&,
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
        new symmetryFvPatchField<Type>{*this, iF}
      };
    }
};
}  // namespace mousse

#include "symmetry_fv_patch_field.ipp"

#endif

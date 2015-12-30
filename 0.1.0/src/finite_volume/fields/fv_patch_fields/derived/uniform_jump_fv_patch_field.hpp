// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformJumpFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a jump condition, using the \c cyclic
//   condition as a base.  The jump is specified as a time-varying uniform
//   value across the patch.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     patchType    | underlying patch type should be \c cyclic| yes |
//     jumpTable    | jump value              | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            uniformJump;
//     patchType       cyclic;
//     jumpTable       constant 10;
//   }
//   \endverbatim
//   The above example shows the use of a fixed jump of '10'.
// Note
//   The uniformValue entry is a DataEntry type, able to describe time
//   varying functions.  The example above gives the usage for supplying a
//   constant value.
//   The underlying \c patchType should be set to \c cyclic
// SeeAlso
//   mousse::fixedJumpFvPatchField
// SourceFiles
//   uniform_jump_fv_patch_field.cpp
#ifndef uniform_jump_fv_patch_field_hpp_
#define uniform_jump_fv_patch_field_hpp_
#include "fixed_jump_fv_patch_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
template<class Type>
class uniformJumpFvPatchField
:
  public fixedJumpFvPatchField<Type>
{
protected:
  // Protected data
    //- "jump" table
    autoPtr<DataEntry<Type> > jumpTable_;
public:
  //- Runtime type information
  TypeName("uniformJump");
  // Constructors
    //- Construct from patch and internal field
    uniformJumpFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformJumpFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given uniformJumpFvPatchField onto a
    //  new patch
    uniformJumpFvPatchField
    (
      const uniformJumpFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformJumpFvPatchField
    (
      const uniformJumpFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new uniformJumpFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    uniformJumpFvPatchField
    (
      const uniformJumpFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new uniformJumpFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    //- Update the coefficients
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "uniform_jump_fv_patch_field.cpp"
#endif
#endif

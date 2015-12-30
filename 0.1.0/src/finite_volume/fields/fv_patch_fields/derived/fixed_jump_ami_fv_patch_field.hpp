// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedJumpAMIFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a jump condition, across non-conformal
//   cyclic path-pairs, employing an arbitraryMeshInterface (AMI).
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
//     type            fixedJumpAMI;
//     patchType       cyclic;
//     jump            uniform 10;
//   }
//   \endverbatim
//   The above example shows the use of a fixed jump of '10'.
// Note
//   The underlying \c patchType should be set to \c cyclicAMI
// SeeAlso
//   mousse::jumpCyclicAMIFvPatchField
// SourceFiles
//   fixed_jump_ami_fv_patch_field.cpp
#ifndef fixed_jump_ami_fv_patch_field_hpp_
#define fixed_jump_ami_fv_patch_field_hpp_
#include "jump_cyclic_ami_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class fixedJumpAMIFvPatchField
:
  public jumpCyclicAMIFvPatchField<Type>
{
protected:
  // Protected data
    //- "jump" field
    Field<Type> jump_;
public:
  //- Runtime type information
  TypeName("fixedJumpAMI");
  // Constructors
    //- Construct from patch and internal field
    fixedJumpAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedJumpAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedJumpAMIFvPatchField onto a
    //  new patch
    fixedJumpAMIFvPatchField
    (
      const fixedJumpAMIFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedJumpAMIFvPatchField
    (
      const fixedJumpAMIFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new fixedJumpAMIFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedJumpAMIFvPatchField
    (
      const fixedJumpAMIFvPatchField<Type>&,
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
        new fixedJumpAMIFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the "jump" across the patch
      virtual tmp<Field<Type> > jump() const;
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap(const fvPatchFieldMapper&);
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap(const fvPatchField<Type>&, const labelList&);
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "fixed_jump_ami_fv_patch_field.cpp"
#endif
#endif

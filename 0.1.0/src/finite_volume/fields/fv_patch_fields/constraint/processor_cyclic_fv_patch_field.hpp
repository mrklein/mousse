// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition enables processor communication across cyclic
//   patches.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            processor;
//   }
//   \endverbatim
// SeeAlso
//   mousse::processorFvPatchField
// SourceFiles
//   processor_cyclic_fv_patch_field.cpp
//   processor_cyclic_fv_patch_fields.hpp
//   processor_cyclic_fv_patch_fields.cpp
//   processor_cyclic_fv_patch_fields_fwd.hpp
#ifndef processor_cyclic_fv_patch_field_hpp_
#define processor_cyclic_fv_patch_field_hpp_
#include "processor_cyclic_fv_patch.hpp"
#include "processor_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class processorCyclicFvPatchField
:
  public processorFvPatchField<Type>
{
  // Private data
    //- Local reference cast into the processor patch
    const processorCyclicFvPatch& procPatch_;
public:
  //- Runtime type information
  TypeName(processorCyclicFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    processorCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch and internal field and patch field
    processorCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    processorCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given processorCyclicFvPatchField onto a
    //  new patch
    processorCyclicFvPatchField
    (
      const processorCyclicFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    processorCyclicFvPatchField(const processorCyclicFvPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new processorCyclicFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    processorCyclicFvPatchField
    (
      const processorCyclicFvPatchField<Type>&,
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
        new processorCyclicFvPatchField<Type>(*this, iF)
      );
    }
  //- Destructor
  virtual ~processorCyclicFvPatchField();
  // Member functions
    // Access
      //- Does the patch field perform the transfromation
      virtual bool doTransform() const
      {
        return !(procPatch_.parallel() || pTraits<Type>::rank == 0);
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return procPatch_.forwardT();
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "processor_cyclic_fv_patch_field.cpp"
#endif
#endif

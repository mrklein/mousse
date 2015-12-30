// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition enforces a cyclic condition between a pair of
//   boundaries.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            cyclic;
//   }
//   \endverbatim
// Note
//   The patches must be topologically similar, i.e. if the owner patch is
//   transformed to the neighbour patch, the patches should be identical (or
//   very similar).
// SourceFiles
//   cyclic_fv_patch_field.cpp
#ifndef cyclic_fv_patch_field_hpp_
#define cyclic_fv_patch_field_hpp_
#include "coupled_fv_patch_field.hpp"
#include "cyclic_ldu_interface_field.hpp"
#include "cyclic_fv_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicFvPatchField
:
  virtual public cyclicLduInterfaceField,
  public coupledFvPatchField<Type>
{
  // Private data
    //- Local reference cast into the cyclic patch
    const cyclicFvPatch& cyclicPatch_;
  // Private Member Functions
    //- Return neighbour side field given internal fields
    template<class Type2>
    tmp<Field<Type2> > neighbourSideField
    (
      const Field<Type2>&
    ) const;
public:
  //- Runtime type information
  TypeName(cyclicFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given cyclicFvPatchField onto a new patch
    cyclicFvPatchField
    (
      const cyclicFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cyclicFvPatchField
    (
      const cyclicFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new cyclicFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cyclicFvPatchField
    (
      const cyclicFvPatchField<Type>&,
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
        new cyclicFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return local reference cast into the cyclic patch
      const cyclicFvPatch& cyclicPatch() const
      {
        return cyclicPatch_;
      }
    // Evaluation functions
      //- Return neighbour coupled internal cell data
      tmp<Field<Type> > patchNeighbourField() const;
      //- Return reference to neighbour patchField
      const cyclicFvPatchField<Type>& neighbourPatchField() const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<Type>& result,
        const Field<Type>& psiInternal,
        const scalarField& coeffs,
        const Pstream::commsTypes commsType
      ) const;
    // Cyclic coupled interface functions
      //- Does the patch field perform the transformation
      virtual bool doTransform() const
      {
        return !(cyclicPatch_.parallel() || pTraits<Type>::rank == 0);
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicPatch_.reverseT();
      }
      //- Return rank of component for transform
      virtual int rank() const
      {
        return pTraits<Type>::rank;
      }
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "cyclic_fv_patch_field.cpp"
#endif
#endif

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition enforces a cyclic condition between a pair of
//   boundaries, whereby communication between the patches is performed using
//   an arbitrary mesh interface (AMI) interpolation.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            cyclicAMI;
//   }
//   \endverbatim
// Note
//   The outer boundary of the patch pairs must be similar, i.e. if the owner
//   patch is transformed to the neighbour patch, the outer perimiter of each
//   patch should be identical (or very similar).
// SeeAlso
//   mousse::AMIInterpolation
// SourceFiles
//   cyclic_ami_fv_patch_field.cpp
#ifndef cyclic_ami_fv_patch_field_hpp_
#define cyclic_ami_fv_patch_field_hpp_
#include "coupled_fv_patch_field.hpp"
#include "cyclic_ami_ldu_interface_field.hpp"
#include "cyclic_ami_fv_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicAMIFvPatchField
:
  virtual public cyclicAMILduInterfaceField,
  public coupledFvPatchField<Type>
{
  // Private data
    //- Local reference cast into the cyclic patch
    const cyclicAMIFvPatch& cyclicAMIPatch_;
  // Private Member Functions
    //- Return neighbour side field given internal fields
    template<class Type2>
    tmp<Field<Type2> > neighbourSideField
    (
      const Field<Type2>&
    ) const;
public:
  //- Runtime type information
  TypeName(cyclicAMIFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given cyclicAMIFvPatchField onto a new patch
    cyclicAMIFvPatchField
    (
      const cyclicAMIFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cyclicAMIFvPatchField(const cyclicAMIFvPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new cyclicAMIFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cyclicAMIFvPatchField
    (
      const cyclicAMIFvPatchField<Type>&,
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
        new cyclicAMIFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return local reference cast into the cyclic AMI patch
      const cyclicAMIFvPatch& cyclicAMIPatch() const
      {
        return cyclicAMIPatch_;
      }
    // Evaluation functions
      //- Return true if coupled. Note that the underlying patch
      //  is not coupled() - the points don't align.
      virtual bool coupled() const;
      //- Return neighbour coupled internal cell data
      virtual tmp<Field<Type> > patchNeighbourField() const;
      //- Return reference to neighbour patchField
      const cyclicAMIFvPatchField<Type>& neighbourPatchField() const;
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
        Field<Type>&,
        const Field<Type>&,
        const scalarField&,
        const Pstream::commsTypes commsType
      ) const;
    // Cyclic AMI coupled interface functions
      //- Does the patch field perform the transformation
      virtual bool doTransform() const
      {
        return
          !(cyclicAMIPatch_.parallel() || pTraits<Type>::rank == 0);
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicAMIPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicAMIPatch_.reverseT();
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
#   include "cyclic_ami_fv_patch_field.cpp"
#endif
#endif

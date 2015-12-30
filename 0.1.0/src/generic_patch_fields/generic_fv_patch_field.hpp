// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericFvPatchField
// Description
//   This boundary condition provides a generic version of the \c calculated
//   condition, useful as a fallback for handling unknown patch types.  Not
//   generally applicable as a user-specified condition.
// SeeAlso
//   mousse::calculatedFvPatchField
// SourceFiles
//   generic_fv_patch_field.cpp
#ifndef generic_fv_patch_field_hpp_
#define generic_fv_patch_field_hpp_
#include "calculated_fv_patch_field.hpp"
#include "hash_ptr_table.hpp"
namespace mousse
{
template<class Type>
class genericFvPatchField
:
  public calculatedFvPatchField<Type>
{
  // Private data
    word actualTypeName_;
    dictionary dict_;
    HashPtrTable<scalarField> scalarFields_;
    HashPtrTable<vectorField> vectorFields_;
    HashPtrTable<sphericalTensorField> sphericalTensorFields_;
    HashPtrTable<symmTensorField> symmTensorFields_;
    HashPtrTable<tensorField> tensorFields_;
public:
  //- Runtime type information
  TypeName("generic");
  // Constructors
    //- Construct from patch and internal field
    genericFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    genericFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    genericFvPatchField
    (
      const genericFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    genericFvPatchField
    (
      const genericFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new genericFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    genericFvPatchField
    (
      const genericFvPatchField<Type>&,
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
        new genericFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
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
    // Evaluation functions
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueInternalCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueBoundaryCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      tmp<Field<Type> > gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      tmp<Field<Type> > gradientBoundaryCoeffs() const;
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "generic_fv_patch_field.cpp"
#endif
#endif

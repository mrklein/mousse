// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zeroGradientFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition applies a zero-gradient condition from the patch
//   internal field onto the patch faces.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            zeroGradient;
//   }
//   \endverbatim
// SourceFiles
//   zero_gradient_fv_patch_field.cpp
#ifndef zero_gradient_fv_patch_field_hpp_
#define zero_gradient_fv_patch_field_hpp_
#include "fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class zeroGradientFvPatchField
:
  public fvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("zeroGradient");
  // Constructors
    //- Construct from patch and internal field
    zeroGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    zeroGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given zeroGradientFvPatchField<Type>
    //  onto a new patch
    zeroGradientFvPatchField
    (
      const zeroGradientFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    zeroGradientFvPatchField
    (
      const zeroGradientFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new zeroGradientFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    zeroGradientFvPatchField
    (
      const zeroGradientFvPatchField<Type>&,
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
        new zeroGradientFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Return gradient at boundary
      virtual tmp<Field<Type> > snGrad() const
      {
        return tmp<Field<Type> >
        (
          new Field<Type>(this->size(), pTraits<Type>::zero)
        );
      }
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
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
      virtual tmp<Field<Type> > gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "zero_gradient_fv_patch_field.cpp"
#endif
#endif

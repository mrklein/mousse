#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_FIXED_GRADIENT_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_FIXED_GRADIENT_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedGradientFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition supplies a fixed gradient condition, such that
//   the patch values are calculated using:
//     \f[
//       x_p = x_c + \frac{\nabla(x)}{\Delta}
//     \f]
//   where
//   \vartable
//     x_p      | patch values
//     x_c      | internal field values
//     \nabla(x)| gradient (user-specified)
//     \Delta   | inverse distance from patch face centre to cell centre
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     gradient     | gradient                | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedGradient;
//     gradient        uniform 0;
//   }
//   \endverbatim

#include "fv_patch_field.hpp"


namespace mousse {

template<class Type>
class fixedGradientFvPatchField
:
  public fvPatchField<Type>
{
  // Private data
    Field<Type> gradient_;
public:
  //- Runtime type information
  TYPE_NAME("fixedGradient");
  // Constructors
    //- Construct from patch and internal field
    fixedGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedGradientFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given fixedGradientFvPatchField
    //  onto a new patch
    fixedGradientFvPatchField
    (
      const fixedGradientFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedGradientFvPatchField
    (
      const fixedGradientFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new fixedGradientFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    fixedGradientFvPatchField
    (
      const fixedGradientFvPatchField<Type>&,
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
        new fixedGradientFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    // Return defining fields
      //- Return gradient at boundary
      virtual Field<Type>& gradient()
      {
        return gradient_;
      }
      virtual const Field<Type>& gradient() const
      {
        return gradient_;
      }
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
      //- Return gradient at boundary
      virtual tmp<Field<Type>> snGrad() const
      {
        return gradient_;
      }
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueInternalCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueBoundaryCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "fixed_gradient_fv_patch_field.ipp"

#endif

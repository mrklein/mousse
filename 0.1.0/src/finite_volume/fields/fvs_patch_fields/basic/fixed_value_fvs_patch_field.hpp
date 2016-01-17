// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedValueFvsPatchField
// Description
//   mousse::fixedValueFvsPatchField
// SourceFiles
//   fixed_value_fvs_patch_field.cpp
#ifndef fixed_value_fvs_patch_field_hpp_
#define fixed_value_fvs_patch_field_hpp_
#include "fvs_patch_field.hpp"
namespace mousse
{
template<class Type>
class fixedValueFvsPatchField
:
  public fvsPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("fixedValue");
  // Constructors
    //- Construct from patch and internal field
    fixedValueFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedValueFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given fixedValueFvsPatchField<Type>
    //  onto a new patch
    fixedValueFvsPatchField
    (
      const fixedValueFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedValueFvsPatchField
    (
      const fixedValueFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new fixedValueFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedValueFvsPatchField
    (
      const fixedValueFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type> > clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type> >
      (
        new fixedValueFvsPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return true if this patch field fixes a value.
      //  Needed to check if a level has to be specified while solving
      //  Poissons equations.
      virtual bool fixesValue() const
      {
        return true;
      }
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
      virtual tmp<Field<Type> > gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;
  // Member operators
    virtual void operator=(const UList<Type>&) {}
    virtual void operator=(const fvsPatchField<Type>&) {}
    virtual void operator+=(const fvsPatchField<Type>&) {}
    virtual void operator-=(const fvsPatchField<Type>&) {}
    virtual void operator*=(const fvsPatchField<scalar>&) {}
    virtual void operator/=(const fvsPatchField<scalar>&) {}
    virtual void operator+=(const Field<Type>&) {}
    virtual void operator-=(const Field<Type>&) {}
    virtual void operator*=(const Field<scalar>&) {}
    virtual void operator/=(const Field<scalar>&) {}
    virtual void operator=(const Type&) {}
    virtual void operator+=(const Type&) {}
    virtual void operator-=(const Type&) {}
    virtual void operator*=(const scalar) {}
    virtual void operator/=(const scalar) {}
};
}  // namespace mousse
#ifdef NoRepository
#   include "fixed_value_fvs_patch_field.cpp"
#endif
#endif

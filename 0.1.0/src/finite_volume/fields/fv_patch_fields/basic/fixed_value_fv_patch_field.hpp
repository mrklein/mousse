#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_FIXED_VALUE_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_FIXED_VALUE_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedValueFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition supplies a fixed value constraint, and is the base
//   class for a number of other boundary conditions.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     value        | Patch face values       | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedValue;
//     value           uniform 0;  // example for scalar field usage
//   }
//   \endverbatim

#include "fv_patch_field.hpp"


namespace mousse {

template<class Type>
class fixedValueFvPatchField
:
  public fvPatchField<Type>
{
public:

  //- Runtime type information
  TYPE_NAME("fixedValue");

  // Constructors

    //- Construct from patch and internal field
    fixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    fixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );

    //- Construct by mapping the given fixedValueFvPatchField<Type>
    //  onto a new patch
    fixedValueFvPatchField
    (
      const fixedValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    fixedValueFvPatchField
    (
      const fixedValueFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new fixedValueFvPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    fixedValueFvPatchField
    (
      const fixedValueFvPatchField<Type>&,
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
        new fixedValueFvPatchField<Type>{*this, iF}
      };
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

  // Member operators
    virtual void operator=(const UList<Type>&) {}
    virtual void operator=(const fvPatchField<Type>&) {}
    virtual void operator+=(const fvPatchField<Type>&) {}
    virtual void operator-=(const fvPatchField<Type>&) {}
    virtual void operator*=(const fvPatchField<scalar>&) {}
    virtual void operator/=(const fvPatchField<scalar>&) {}
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

#include "fixed_value_fv_patch_field.ipp"

#endif

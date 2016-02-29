#ifndef SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_MIXED_FIXED_VALUE_SLIP_FV_PATCH_FIELD_HPP_
#define SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_MIXED_FIXED_VALUE_SLIP_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixedFixedValueSlipFvPatchField
// Description
//   A mixed boundary type that blends between fixedValue and slip, as opposed
//   to the standard mixed condition that blends between fixedValue and
//   fixedGradient; required to implement maxwellSlipU condition.
// SourceFiles
//   mixed_fixed_value_slip_fv_patch_field.cpp
#include "transform_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class mixedFixedValueSlipFvPatchField
:
  public transformFvPatchField<Type>
{
  // Private data
    //- Value field used for boundary condition
    Field<Type> refValue_;
    //- Fraction (0-1) of value used for boundary condition
    scalarField valueFraction_;
public:
  //- Runtime type information
  TYPE_NAME("mixedFixedValueSlip");
  // Constructors
    //- Construct from patch and internal field
    mixedFixedValueSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mixedFixedValueSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given mixedFixedValueSlipFvPatchField
    //- Onto a new patch
    mixedFixedValueSlipFvPatchField
    (
      const mixedFixedValueSlipFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mixedFixedValueSlipFvPatchField
    (
      const mixedFixedValueSlipFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new mixedFixedValueSlipFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    mixedFixedValueSlipFvPatchField
    (
      const mixedFixedValueSlipFvPatchField<Type>&,
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
        new mixedFixedValueSlipFvPatchField<Type>(*this, iF)
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
    // Return defining fields
      virtual Field<Type>& refValue()
      {
        return refValue_;
      }
      virtual const Field<Type>& refValue() const
      {
        return refValue_;
      }
      virtual scalarField& valueFraction()
      {
        return valueFraction_;
      }
      virtual const scalarField& valueFraction() const
      {
        return valueFraction_;
      }
    // Evaluation functions
      //- Return gradient at boundary
      virtual tmp<Field<Type> > snGrad() const;
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
      //- Return face-gradient transform diagonal
      virtual tmp<Field<Type> > snGradTransformDiag() const;
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
#ifdef NoRepository
  #include "mixed_fixed_value_slip_fv_patch_field.cpp"
#endif
#endif

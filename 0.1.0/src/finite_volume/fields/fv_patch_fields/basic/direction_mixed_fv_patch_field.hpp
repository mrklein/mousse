#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_DIRECTION_MIXED_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_DIRECTION_MIXED_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directionMixedFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   Base class for direction-mixed boundary conditions.
// SourceFiles
//   direction_mixed_fv_patch_field.cpp
#include "transform_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class directionMixedFvPatchField
:
  public transformFvPatchField<Type>
{
  // Private data
    //- Value field
    Field<Type> refValue_;
    //- Normal gradient field
    Field<Type> refGrad_;
    //- Fraction (0-1) of value used for boundary condition
    symmTensorField valueFraction_;
public:
  //- Runtime type information
  TYPE_NAME("directionMixed");
  // Constructors
    //- Construct from patch and internal field
    directionMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    directionMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given directionMixedFvPatchField onto
    //  a new patch
    directionMixedFvPatchField
    (
      const directionMixedFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new directionMixedFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    directionMixedFvPatchField
    (
      const directionMixedFvPatchField<Type>&,
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
        new directionMixedFvPatchField<Type>(*this, iF)
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
      virtual Field<Type>& refGrad()
      {
        return refGrad_;
      }
      virtual const Field<Type>& refGrad() const
      {
        return refGrad_;
      }
      virtual symmTensorField& valueFraction()
      {
        return valueFraction_;
      }
      virtual const symmTensorField& valueFraction() const
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
    virtual void operator=(const fvPatchField<Type>&) {}
    virtual void operator+=(const fvPatchField<Type>&) {}
    virtual void operator-=(const fvPatchField<Type>&) {}
    virtual void operator*=(const fvPatchField<Type>&) {}
    virtual void operator/=(const fvPatchField<Type>&) {}
    virtual void operator=(const Field<Type>&) {}
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
#   include "direction_mixed_fv_patch_field.cpp"
#endif
#endif

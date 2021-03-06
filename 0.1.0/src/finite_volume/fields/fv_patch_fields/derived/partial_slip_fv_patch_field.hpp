#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PARTIAL_SLIP_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PARTIAL_SLIP_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::partialSlipFvPatchField
// Group
//   grpWallBoundaryConditions grpGenericBoundaryConditions
// Description
//   This boundary condition provides a partial slip condition.  The amount of
//   slip is controlled by a user-supplied field.
//   \heading Patch usage
//   \table
//     Property      | Description             | Required    | Default value
//     valueFraction | fraction od value used for boundary [0-1] | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            partialSlip;
//     valueFraction   uniform 0.1;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::transformFvPatchField

#include "transform_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class partialSlipFvPatchField
:
  public transformFvPatchField<Type>
{
  // Private data
    //- Fraction (0-1) of value used for boundary condition
    scalarField valueFraction_;
public:
  //- Runtime type information
  TYPE_NAME("partialSlip");
  // Constructors
    //- Construct from patch and internal field
    partialSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    partialSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given partialSlipFvPatchField onto a new patch
    partialSlipFvPatchField
    (
      const partialSlipFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    partialSlipFvPatchField
    (
      const partialSlipFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new partialSlipFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    partialSlipFvPatchField
    (
      const partialSlipFvPatchField<Type>&,
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
        new partialSlipFvPatchField<Type>{*this, iF}
      };
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
      virtual tmp<Field<Type>> snGrad() const;
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
      //- Return face-gradient transform diagonal
      virtual tmp<Field<Type>> snGradTransformDiag() const;
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

#include "partial_slip_fv_patch_field.ipp"

#endif

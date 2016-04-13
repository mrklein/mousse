#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_CALCULATED_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_CALCULATED_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calculatedFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition is not designed to be evaluated; it is assmued
//   that the value is assigned via field assignment, and not via a call to
//   e.g. \c updateCoeffs or \c evaluate.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            calculated;
//     value           uniform (0 0 0);    // optional value entry
//   }
//   \endverbatim

#include "fv_patch_field.hpp"


namespace mousse {

template<class Type>
class calculatedFvPatchField
:
  public fvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("calculated");
  // Constructors
    //- Construct from patch and internal field
    calculatedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    calculatedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&,
      const bool valueRequired=false
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    calculatedFvPatchField
    (
      const calculatedFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    calculatedFvPatchField
    (
      const calculatedFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>{new calculatedFvPatchField<Type>{*this}};
    }
    //- Construct as copy setting internal field reference
    calculatedFvPatchField
    (
      const calculatedFvPatchField<Type>&,
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
        new calculatedFvPatchField<Type>{*this, iF}
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
      tmp<Field<Type>> gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      tmp<Field<Type>> gradientBoundaryCoeffs() const;
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "calculated_fv_patch_field.ipp"

#endif

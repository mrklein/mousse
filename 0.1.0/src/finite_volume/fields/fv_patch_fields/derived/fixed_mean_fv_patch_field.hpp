#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_MEAN_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_MEAN_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedMeanFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition extrapolates field to the patch using the near-cell
//   values and adjusts the distribution to match the specified mean value.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     meanValue    | mean value              | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedMean;
//     meanValue       1.0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   fixed_mean_fv_patch_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
template<class Type>
class fixedMeanFvPatchField
:
  public fixedValueFvPatchField<Type>
{
protected:
  // Protected data
    //- MeanValue value the field is adjusted to maintain
    Type meanValue_;
public:
  //- Runtime type information
  TYPE_NAME("fixedMean");
  // Constructors
    //- Construct from patch and internal field
    fixedMeanFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedMeanFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedMeanFvPatchField
    //  onto a new patch
    fixedMeanFvPatchField
    (
      const fixedMeanFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedMeanFvPatchField
    (
      const fixedMeanFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new fixedMeanFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedMeanFvPatchField
    (
      const fixedMeanFvPatchField<Type>&,
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
        new fixedMeanFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "fixed_mean_fv_patch_field.cpp"
#endif
#endif

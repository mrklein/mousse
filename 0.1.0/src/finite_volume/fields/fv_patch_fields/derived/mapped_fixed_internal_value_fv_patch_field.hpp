// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedFixedInternalValueFvPatchField
// Group
//   grpGenericBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition maps the boundary and internal values of a
//   neighbour patch field to the boundary and internal values of *this.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     fieldName    | name of field to be mapped | no       | this field name
//     setAverage   | flag to activate setting of average value | yes |
//     average      | average value to apply if \c setAverage = yes | yes |
//   \endtable
//   \verbatim
//   myPatch
//   {
//     type            mappedFixedInternalValue;
//     fieldName       T;
//     setAverage      no;
//     average         0;
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   This boundary condition can only be applied to patches that are of
//   the \c mappedPolyPatch type.
// SeeAlso
//   mousse::mappedPatchBase
//   mousse::mappedPolyPatch
//   mousse::mappedFvPatch
//   mousse::mappedFixedValueFvPatchField
// SourceFiles
//   mapped_fixed_internal_value_fv_patch_field.cpp
#ifndef mapped_fixed_internal_value_fv_patch_field_hpp_
#define mapped_fixed_internal_value_fv_patch_field_hpp_
#include "mapped_fixed_value_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class mappedFixedInternalValueFvPatchField
:
  public mappedFixedValueFvPatchField<Type>
{
public:
  //- Runtime type information
  TypeName("mappedFixedInternalValue");
  // Constructors
    //- Construct from patch and internal field
    mappedFixedInternalValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mappedFixedInternalValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // mappedFixedInternalValueFvPatchField  onto a new patch
    mappedFixedInternalValueFvPatchField
    (
      const mappedFixedInternalValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mappedFixedInternalValueFvPatchField
    (
      const mappedFixedInternalValueFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new mappedFixedInternalValueFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    mappedFixedInternalValueFvPatchField
    (
      const mappedFixedInternalValueFvPatchField<Type>&,
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
        new mappedFixedInternalValueFvPatchField<Type>(*this, iF)
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
#   include "mapped_fixed_internal_value_fv_patch_field.cpp"
#endif
#endif

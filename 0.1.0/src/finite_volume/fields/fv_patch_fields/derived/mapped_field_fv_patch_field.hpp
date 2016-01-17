// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedFieldFvPatchField
// Group
//   grpGenericBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a self-contained version of the \c mapped
//   condition.  It does not use information on the patch; instead it holds
//   thr data locally.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     fieldName    | name of field to be mapped | no       | this field name
//     setAverage   | flag to activate setting of average value | yes |
//     average      | average value to apply if \c setAverage = yes | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            mappedField;
//     fieldName       T;              // optional field name
//     setAverage      no;             // apply an average value
//     average         0;              // average to apply if setAverage
//     value           uniform 0;      // place holder
//   }
//   \endverbatim
// Note
//   Since this condition can be applied on a per-field and per-patch basis,
//   it is possible to duplicate the mapping information.  If possible, employ
//   the \c mapped condition in preference to avoid this situation, and only
//   employ this condition if it is not possible to change the underlying
//   geometric (poly) patch type to \c mapped.
// SeeAlso
//   mousse::mappedPatchBase
//   mousse::mappedPolyPatch
//   mousse::mappedFvPatch
//   mousse::fixedValueFvPatchField
// SourceFiles
//   mapped_field_fv_patch_field.cpp
#ifndef mapped_field_fv_patch_field_hpp_
#define mapped_field_fv_patch_field_hpp_
#include "mapped_patch_base.hpp"
#include "mapped_patch_field_base.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "interpolation.hpp"
namespace mousse
{
template<class Type>
class mappedFieldFvPatchField
:
  public fixedValueFvPatchField<Type>,
  public mappedPatchBase,
  public mappedPatchFieldBase<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("mappedField");
  // Constructors
    //- Construct from patch and internal field
    mappedFieldFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mappedFieldFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct from patch, internal field and distance for normal type
    //  sampling
    mappedFieldFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      // mappedPatchBase
      const word& sampleRegion,
      const sampleMode sampleMode,
      const word& samplePatch,
      const scalar distance,
      // My settings
      const word& fieldName,
      const bool setAverage,
      const Type average,
      const word& interpolationScheme
    );
    //- Construct by mapping given
    //  mappedFieldFvPatchField
    //  onto a new patch
    mappedFieldFvPatchField
    (
      const mappedFieldFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mappedFieldFvPatchField
    (
      const mappedFieldFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new mappedFieldFvPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    mappedFieldFvPatchField
    (
      const mappedFieldFvPatchField<Type>&,
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
        new mappedFieldFvPatchField<Type>
        (
          *this,
          iF
        )
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
#   include "mapped_field_fv_patch_field.cpp"
#endif
#endif

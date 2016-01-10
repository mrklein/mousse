// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedFixedValueFvPatchField
// Group
//   grpGenericBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition maps the value at a set of cells or patch faces
//   back to *this.
//   The sample mode is set by the underlying mapping engine, provided by the
//   mappedPatchBase class.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     fieldName    | name of field to be mapped | no       | this field name
//     setAverage   | flag to activate setting of average value | yes |
//     average      | average value to apply if \c setAverage = yes | yes |
//     interpolationScheme | type of interpolation scheme | no |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            mapped;
//     fieldName       T;
//     setAverage      no;
//     average         0;
//     interpolationScheme cell;
//     value           uniform 0;
//   }
//   \endverbatim
//   When employing the \c nearestCell sample mode, the user must also specify
//   the interpolation scheme using the \c interpolationScheme entry.
//   In case of interpolation (where scheme != cell) the limitation is that
//   there is only one value per cell.  For example, if you have a cell with two
//   boundary faces and both faces sample into the cell, both faces will get the
//   same value.
// Note
//   It is not possible to sample internal faces since volume fields are not
//   defined on faces.
// SeeAlso
//   mousse::mappedPatchBase
//   mousse::interpolation
//   mousse::fixedValueFvPatchField
// SourceFiles
//   mapped_fixed_value_fv_patch_field.cpp

#ifndef mapped_fixed_value_fv_patch_field_hpp_
#define mapped_fixed_value_fv_patch_field_hpp_

#include "fixed_value_fv_patch_fields.hpp"
#include "mapped_patch_field_base.hpp"

namespace mousse
{

template<class Type>
class mappedFixedValueFvPatchField
:
  public fixedValueFvPatchField<Type>,
  public mappedPatchFieldBase<Type>
{

protected:

  // Protected Member Functions
    const mappedPatchBase& mapper
    (
      const fvPatch& p,
      const DimensionedField<Type, volMesh>& iF
    );

public:

  //- Runtime type information
  TYPE_NAME("mapped");

  // Constructors

    //- Construct from patch and internal field
    mappedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    mappedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );

    //- Construct by mapping given mappedFixedValueFvPatchField
    //  onto a new patch
    mappedFixedValueFvPatchField
    (
      const mappedFixedValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    mappedFixedValueFvPatchField
    (
      const mappedFixedValueFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      {
        new mappedFixedValueFvPatchField<Type>(*this)
      };
    }

    //- Construct as copy setting internal field reference
    mappedFixedValueFvPatchField
    (
      const mappedFixedValueFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      {
        new mappedFixedValueFvPatchField<Type>{*this, iF}
      };
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
#   include "mapped_fixed_value_fv_patch_field.cpp"
#endif
#endif

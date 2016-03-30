#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_VALUE_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_VALUE_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformFixedValuePointPatchField
// Description
//   Enables the specification of a uniform fixed value boundary condition.
//   Example of the boundary condition specification:
//   \verbatim
//   inlet
//   {
//     type            uniformFixedValue;
//     uniformValue    constant 0.2;
//   }
//   \endverbatim
//   The uniformValue entry is a DataEntry type, able to describe time
//   varying functions.  The example above gives the usage for supplying a
//   constant value.

#include "fixed_value_point_patch_field.hpp"
#include "data_entry.hpp"


namespace mousse {

template<class Type>
class uniformFixedValuePointPatchField
:
  public fixedValuePointPatchField<Type>
{
  // Private data
    autoPtr<DataEntry<Type> > uniformValue_;
public:
  //- Runtime type information
  TYPE_NAME("uniformFixedValue");
  // Constructors
    //- Construct from patch and internal field
    uniformFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    uniformFixedValuePointPatchField
    (
      const uniformFixedValuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    uniformFixedValuePointPatchField
    (
      const uniformFixedValuePointPatchField<Type>&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new uniformFixedValuePointPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    uniformFixedValuePointPatchField
    (
      const uniformFixedValuePointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type>> clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new uniformFixedValuePointPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the fluctuation scale
      const DataEntry<Type>& uniformValue() const
      {
        return uniformValue_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#include "uniform_fixed_value_point_patch_field.ipp"

#endif

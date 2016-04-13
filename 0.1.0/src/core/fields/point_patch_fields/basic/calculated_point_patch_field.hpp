#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_CALCULATED_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_CALCULATED_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calculatedPointPatchField
// Description
//   A calculated boundary condition for pointField

#include "point_patch_field.hpp"


namespace mousse {

template<class Type>
class calculatedPointPatchField
:
  public pointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("calculated");
  // Constructors
    //- Construct from patch and internal field
    calculatedPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    calculatedPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    calculatedPointPatchField
    (
      const calculatedPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new calculatedPointPatchField<Type>
        {
          *this
        }
      };
    }
    //- Construct as copy setting internal field reference
    calculatedPointPatchField
    (
      const calculatedPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type>>
    clone(const DimensionedField<Type, pointMesh>& iF) const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new calculatedPointPatchField<Type>
        {
          *this,
          iF
        }
      };
    }
};
}  // namespace mousse

#include "calculated_point_patch_field.ipp"

#endif

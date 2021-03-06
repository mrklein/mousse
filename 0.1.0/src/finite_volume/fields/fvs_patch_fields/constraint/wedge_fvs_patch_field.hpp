#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_WEDGE_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_WEDGE_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgeFvsPatchField
// Description
//   mousse::wedgeFvsPatchField

#include "fvs_patch_field.hpp"
#include "wedge_fv_patch.hpp"


namespace mousse {

template<class Type>
class wedgeFvsPatchField
:
  public fvsPatchField<Type>
{
public:

  //- Runtime type information
  TYPE_NAME(wedgeFvPatch::typeName_());

  // Constructors

    //- Construct from patch and internal field
    wedgeFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct from patch, internal field and dictionary
    wedgeFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );

    //- Construct by mapping given wedgeFvsPatchField onto a new patch
    wedgeFvsPatchField
    (
      const wedgeFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    wedgeFvsPatchField
    (
      const wedgeFvsPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type>> clone() const
    {
      return tmp<fvsPatchField<Type>>
      {
        new wedgeFvsPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    wedgeFvsPatchField
    (
      const wedgeFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type>> clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type>>
      {
        new wedgeFvsPatchField<Type>{*this, iF}
      };
    }

};

}  // namespace mousse

#include "wedge_fvs_patch_field.ipp"

#endif

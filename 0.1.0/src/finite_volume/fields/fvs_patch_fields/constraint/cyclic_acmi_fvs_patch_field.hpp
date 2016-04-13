#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_CYCLIC_ACMI_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_CYCLIC_ACMI_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIFvsPatchField
// Description
//   mousse::cyclicACMIFvsPatchField

#include "coupled_fvs_patch_field.hpp"
#include "cyclic_acmi_fv_patch.hpp"


namespace mousse {

template<class Type>
class cyclicACMIFvsPatchField
:
  public coupledFvsPatchField<Type>
{
  // Private data

    //- Local reference cast into the cyclic patch
    const cyclicACMIFvPatch& cyclicACMIPatch_;

public:

  //- Runtime type information
  TYPE_NAME(cyclicACMIFvPatch::typeName_());

  // Constructors

    //- Construct from patch and internal field
    cyclicACMIFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct from patch, internal field and dictionary
    cyclicACMIFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );

    //- Construct by mapping given cyclicACMIFvsPatchField onto a new patch
    cyclicACMIFvsPatchField
    (
      const cyclicACMIFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    cyclicACMIFvsPatchField
    (
      const cyclicACMIFvsPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type>> clone() const
    {
      return tmp<fvsPatchField<Type>>
      {
        new cyclicACMIFvsPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    cyclicACMIFvsPatchField
    (
      const cyclicACMIFvsPatchField<Type>&,
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
        new cyclicACMIFvsPatchField<Type>{*this, iF}
      };
    }

  // Member functions

    // Access

      //- Return true if running parallel
      virtual bool coupled() const;
};

}  // namespace mousse

#include "cyclic_acmi_fvs_patch_field.ipp"

#endif

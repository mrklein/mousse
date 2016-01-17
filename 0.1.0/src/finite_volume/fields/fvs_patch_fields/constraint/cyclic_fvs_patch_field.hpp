// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicFvsPatchField
// Description
//   mousse::cyclicFvsPatchField
// SourceFiles
//   cyclic_fvs_patch_field.cpp

#ifndef cyclic_fvs_patch_field_hpp_
#define cyclic_fvs_patch_field_hpp_

#include "coupled_fvs_patch_field.hpp"
#include "cyclic_fv_patch.hpp"

namespace mousse
{

template<class Type>
class cyclicFvsPatchField
:
  public coupledFvsPatchField<Type>
{
  // Private data

    //- Local reference cast into the cyclic patch
    const cyclicFvPatch& cyclicPatch_;

public:

  //- Runtime type information
  TYPE_NAME(cyclicFvPatch::typeName_());

  // Constructors

    //- Construct from patch and internal field
    cyclicFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct from patch, internal field and dictionary
    cyclicFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );

    //- Construct by mapping given cyclicFvsPatchField onto a new patch
    cyclicFvsPatchField
    (
      const cyclicFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    cyclicFvsPatchField
    (
      const cyclicFvsPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new cyclicFvsPatchField<Type>(*this)
      );
    }

    //- Construct as copy setting internal field reference
    cyclicFvsPatchField
    (
      const cyclicFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type> > clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type> >
      {
        new cyclicFvsPatchField<Type>{*this, iF}
      };
    }
};

}  // namespace mousse

#ifdef NoRepository
#   include "cyclic_fvs_patch_field.cpp"
#endif
#endif

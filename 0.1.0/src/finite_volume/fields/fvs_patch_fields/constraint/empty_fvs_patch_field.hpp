// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyFvsPatchField
// Description
//   mousse::emptyFvsPatchField
// SourceFiles
//   empty_fvs_patch_field.cpp

#ifndef empty_fvs_patch_field_hpp_
#define empty_fvs_patch_field_hpp_

#include "fvs_patch_field.hpp"
#include "empty_fv_patch.hpp"

namespace mousse
{

template<class Type>
class emptyFvsPatchField
:
  public fvsPatchField<Type>
{
public:

  //- Runtime type information
  TYPE_NAME(emptyFvPatch::typeName_());

  // Constructors

    //- Construct from patch and internal field
    emptyFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct from patch, internal field and dictionary
    emptyFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );

    //- Construct by mapping given emptyFvsPatchField onto a new patch
    emptyFvsPatchField
    (
      const emptyFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    emptyFvsPatchField
    (
      const emptyFvsPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      {
        new emptyFvsPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    emptyFvsPatchField
    (
      const emptyFvsPatchField<Type>&,
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
        new emptyFvsPatchField<Type>{*this, iF}
      };
    }

  // Member functions

    // Mapping functions

      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      )
      {}

      //- Reverse map the given fvsPatchField onto this fvsPatchField
      virtual void rmap
      (
        const fvsPatchField<Type>&,
        const labelList&
      )
      {}

};

}  // namespace mousse
#ifdef NoRepository
#   include "empty_fvs_patch_field.cpp"
#endif
#endif

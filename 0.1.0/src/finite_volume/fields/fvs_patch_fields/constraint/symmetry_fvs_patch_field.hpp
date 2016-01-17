// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryFvsPatchField
// Description
//   mousse::symmetryFvsPatchField
// SourceFiles
//   symmetry_fvs_patch_field.cpp

#ifndef symmetry_fvs_patch_field_hpp_
#define symmetry_fvs_patch_field_hpp_

#include "fvs_patch_field.hpp"
#include "symmetry_fv_patch.hpp"

namespace mousse
{
template<class Type>
class symmetryFvsPatchField
:
  public fvsPatchField<Type>
{
public:

  //- Runtime type information
  TYPE_NAME(symmetryFvPatch::typeName_());

  // Constructors

    //- Construct from patch and internal field
    symmetryFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );

    //- Construct from patch, internal field and dictionary
    symmetryFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );

    //- Construct by mapping given symmetryFvsPatchField onto a new patch
    symmetryFvsPatchField
    (
      const symmetryFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    symmetryFvsPatchField
    (
      const symmetryFvsPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      {
        new symmetryFvsPatchField<Type>{*this}
      };
    }

    //- Construct as copy setting internal field reference
    symmetryFvsPatchField
    (
      const symmetryFvsPatchField<Type>&,
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
        new symmetryFvsPatchField<Type>{*this, iF}
      };
    }

};

}  // namespace mousse
#ifdef NoRepository
#   include "symmetry_fvs_patch_field.cpp"
#endif
#endif

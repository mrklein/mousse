#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_BASIC_CALCULATED_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_BASIC_CALCULATED_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calculatedFvsPatchField
// Description
//   mousse::calculatedFvsPatchField
// SourceFiles
//   calculated_fvs_patch_field.cpp
#include "fvs_patch_field.hpp"
namespace mousse
{
template<class Type>
class calculatedFvsPatchField
:
  public fvsPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("calculated");
  // Constructors
    //- Construct from patch and internal field
    calculatedFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    calculatedFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    calculatedFvsPatchField
    (
      const calculatedFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    calculatedFvsPatchField
    (
      const calculatedFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new calculatedFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    calculatedFvsPatchField
    (
      const calculatedFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type> > clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type> >
      (
        new calculatedFvsPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return true if this patch field fixes a value.
      //  Needed to check if a level has to be specified while solving
      //  Poissons equations.
      virtual bool fixesValue() const
      {
        return true;
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "calculated_fvs_patch_field.cpp"
#endif
#endif

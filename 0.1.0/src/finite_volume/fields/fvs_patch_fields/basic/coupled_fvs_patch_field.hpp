// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledFvsPatchField
// Description
//   mousse::coupledFvsPatchField
// SourceFiles
//   coupled_fvs_patch_field.cpp
#ifndef coupled_fvs_patch_field_hpp_
#define coupled_fvs_patch_field_hpp_
#include "fvs_patch_field.hpp"
#include "coupled_fv_patch.hpp"
namespace mousse
{
template<class Type>
class coupledFvsPatchField
:
  public fvsPatchField<Type>
{
public:
  //- Runtime type information
  TypeName(coupledFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    coupledFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch and internal field and patch field
    coupledFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    coupledFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given coupledFvsPatchField onto a new patch
    coupledFvsPatchField
    (
      const coupledFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    coupledFvsPatchField
    (
      const coupledFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const = 0;
    //- Construct as copy setting internal field reference
    coupledFvsPatchField
    (
      const coupledFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone
    (
      const DimensionedField<Type, surfaceMesh>&
    ) const = 0;
  // Member functions
    // Access
      //- Return true if this patch field is derived from
      //  coupledFvsPatchField<Type>.
      virtual bool coupled() const
      {
        return true;
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "coupled_fvs_patch_field.cpp"
#endif
#endif

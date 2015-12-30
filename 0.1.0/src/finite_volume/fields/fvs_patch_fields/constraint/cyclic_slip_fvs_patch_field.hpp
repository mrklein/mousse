// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicSlipFvsPatchField
// Description
//   mousse::cyclicSlipFvsPatchField
// SourceFiles
//   cyclic_slip_fvs_patch_field.cpp
#ifndef cyclic_slip_fvs_patch_field_hpp_
#define cyclic_slip_fvs_patch_field_hpp_
#include "cyclic_fvs_patch_field.hpp"
#include "cyclic_slip_fv_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicSlipFvsPatchField
:
  public cyclicFvsPatchField<Type>
{
public:
  //- Runtime type information
  TypeName(cyclicSlipFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicSlipFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicSlipFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given cyclicSlipFvsPatchField onto a new patch
    cyclicSlipFvsPatchField
    (
      const cyclicSlipFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cyclicSlipFvsPatchField
    (
      const cyclicSlipFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new cyclicSlipFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cyclicSlipFvsPatchField
    (
      const cyclicSlipFvsPatchField<Type>&,
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
        new cyclicSlipFvsPatchField<Type>(*this, iF)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "cyclic_slip_fvs_patch_field.cpp"
#endif
#endif

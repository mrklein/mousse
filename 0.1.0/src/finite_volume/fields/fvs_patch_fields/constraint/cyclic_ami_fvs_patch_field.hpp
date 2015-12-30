// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIFvsPatchField
// Description
//   mousse::cyclicAMIFvsPatchField
// SourceFiles
//   cyclic_ami_fvs_patch_field.cpp
#ifndef cyclic_ami_fvs_patch_field_hpp_
#define cyclic_ami_fvs_patch_field_hpp_
#include "coupled_fvs_patch_field.hpp"
#include "cyclic_ami_fv_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicAMIFvsPatchField
:
  public coupledFvsPatchField<Type>
{
  // Private data
    //- Local reference cast into the cyclic patch
    const cyclicAMIFvPatch& cyclicAMIPatch_;
public:
  //- Runtime type information
  TypeName(cyclicAMIFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicAMIFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicAMIFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given cyclicAMIFvsPatchField onto a new patch
    cyclicAMIFvsPatchField
    (
      const cyclicAMIFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cyclicAMIFvsPatchField
    (
      const cyclicAMIFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new cyclicAMIFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cyclicAMIFvsPatchField
    (
      const cyclicAMIFvsPatchField<Type>&,
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
        new cyclicAMIFvsPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return true if running parallel
      virtual bool coupled() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "cyclic_ami_fvs_patch_field.cpp"
#endif
#endif

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorFvsPatchField
// Description
//   mousse::processorFvsPatchField
// SourceFiles
//   processor_fvs_patch_field.cpp
#ifndef processor_fvs_patch_field_hpp_
#define processor_fvs_patch_field_hpp_
#include "coupled_fvs_patch_field.hpp"
#include "processor_fv_patch.hpp"
namespace mousse
{
template<class Type>
class processorFvsPatchField
:
  public coupledFvsPatchField<Type>
{
  // Private data
    //- Local reference cast into the processor patch
    const processorFvPatch& procPatch_;
public:
  //- Runtime type information
  TypeName(processorFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    processorFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch and internal field and patch field
    processorFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    processorFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given processorFvsPatchField onto a new patch
    processorFvsPatchField
    (
      const processorFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    processorFvsPatchField(const processorFvsPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new processorFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    processorFvsPatchField
    (
      const processorFvsPatchField<Type>&,
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
        new processorFvsPatchField<Type>(*this, iF)
      );
    }
  //- Destructor
  virtual ~processorFvsPatchField();
  // Member functions
    // Access
      //- Return true if running parallel
      virtual bool coupled() const
      {
        if (Pstream::parRun())
        {
          return true;
        }
        else
        {
          return false;
        }
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "processor_fvs_patch_field.cpp"
#endif
#endif

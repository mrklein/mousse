#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonuniformTransformCyclicFvsPatchField
// Description
//   mousse::nonuniformTransformCyclicFvsPatchField
// SourceFiles
//   nonuniform_transform_cyclic_fvs_patch_field.cpp
#include "cyclic_fvs_patch_field.hpp"
#include "nonuniform_transform_cyclic_fv_patch.hpp"
namespace mousse
{
template<class Type>
class nonuniformTransformCyclicFvsPatchField
:
  public cyclicFvsPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(nonuniformTransformCyclicFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    nonuniformTransformCyclicFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nonuniformTransformCyclicFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nonuniformTransformCyclicFvsPatchField onto a new patch
    nonuniformTransformCyclicFvsPatchField
    (
      const nonuniformTransformCyclicFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nonuniformTransformCyclicFvsPatchField
    (
      const nonuniformTransformCyclicFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new nonuniformTransformCyclicFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nonuniformTransformCyclicFvsPatchField
    (
      const nonuniformTransformCyclicFvsPatchField<Type>&,
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
        new nonuniformTransformCyclicFvsPatchField<Type>(*this, iF)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "nonuniform_transform_cyclic_fvs_patch_field.cpp"
#endif
#endif

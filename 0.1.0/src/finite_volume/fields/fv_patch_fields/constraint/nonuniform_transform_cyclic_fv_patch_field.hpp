// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonuniformTransformCyclicFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition enforces a cyclic condition between a pair of
//   boundaries, incorporating a non-uniform transformation.
// SourceFiles
//   nonuniform_transform_cyclic_fv_patch_field.cpp
//   nonuniform_transform_cyclic_fv_patch_fields.hpp
//   nonuniform_transform_cyclic_fv_patch_fields.cpp
//   nonuniform_transform_cyclic_fv_patch_fields_fwd.hpp
#ifndef nonuniform_transform_cyclic_fv_patch_field_hpp_
#define nonuniform_transform_cyclic_fv_patch_field_hpp_
#include "cyclic_fv_patch_field.hpp"
#include "nonuniform_transform_cyclic_fv_patch.hpp"
namespace mousse
{
template<class Type>
class nonuniformTransformCyclicFvPatchField
:
  public cyclicFvPatchField<Type>
{
  // Private data
public:
  //- Runtime type information
  TYPE_NAME(nonuniformTransformCyclicFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    nonuniformTransformCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nonuniformTransformCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nonuniformTransformCyclicFvPatchField onto a new patch
    nonuniformTransformCyclicFvPatchField
    (
      const nonuniformTransformCyclicFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nonuniformTransformCyclicFvPatchField
    (
      const nonuniformTransformCyclicFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new nonuniformTransformCyclicFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nonuniformTransformCyclicFvPatchField
    (
      const nonuniformTransformCyclicFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new nonuniformTransformCyclicFvPatchField<Type>(*this, iF)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "nonuniform_transform_cyclic_fv_patch_field.cpp"
#endif
#endif

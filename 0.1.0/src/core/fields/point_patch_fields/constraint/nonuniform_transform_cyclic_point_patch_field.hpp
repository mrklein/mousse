#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonuniformTransformCyclicPointPatchField
// Description
//   Cyclic + slip constraints
// SourceFiles
//   nonuniform_transform_cyclic_point_patch_field.cpp

#include "cyclic_point_patch_field.hpp"
#include "nonuniform_transform_cyclic_point_patch.hpp"

namespace mousse
{
template<class Type>
class nonuniformTransformCyclicPointPatchField
:
  public cyclicPointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(nonuniformTransformCyclicPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    nonuniformTransformCyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nonuniformTransformCyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    nonuniformTransformCyclicPointPatchField
    (
      const nonuniformTransformCyclicPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new nonuniformTransformCyclicPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    nonuniformTransformCyclicPointPatchField
    (
      const nonuniformTransformCyclicPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new nonuniformTransformCyclicPointPatchField<Type>
        (
          *this, iF
        )
      );
    }
  // Member functions
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
};
}  // namespace mousse
#ifdef NoRepository
#    include "nonuniform_transform_cyclic_point_patch_field.cpp"
#endif
#endif

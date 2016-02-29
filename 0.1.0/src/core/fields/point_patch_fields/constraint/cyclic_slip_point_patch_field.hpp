#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_CYCLIC_SLIP_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_CYCLIC_SLIP_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicSlipPointPatchField
// Description
//   Cyclic + slip constraints
// SourceFiles
//   cyclic_slip_point_patch_field.cpp
#include "cyclic_point_patch_field.hpp"
#include "cyclic_slip_point_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicSlipPointPatchField
:
  public cyclicPointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(cyclicSlipPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicSlipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicSlipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    cyclicSlipPointPatchField
    (
      const cyclicSlipPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new cyclicSlipPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    cyclicSlipPointPatchField
    (
      const cyclicSlipPointPatchField<Type>&,
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
        new cyclicSlipPointPatchField<Type>
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
#    include "cyclic_slip_point_patch_field.cpp"
#endif
#endif

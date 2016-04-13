#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_FIXED_NORMAL_SLIP_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_FIXED_NORMAL_SLIP_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedNormalSlipPointPatchField
// Description
//   slip with user-specified normal

#include "slip_point_patch_field.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class Type>
class fixedNormalSlipPointPatchField;

template<class Type>
Ostream& operator<<
(
  Ostream&,
  const fixedNormalSlipPointPatchField<Type>&
);


template<class Type>
class fixedNormalSlipPointPatchField
:
  public slipPointPatchField<Type>
{
  // Private data
    //- User specified normal
    vector n_;
public:
  //- Runtime type information
  TYPE_NAME("fixedNormalSlip");
  // Constructors
    //- Construct from patch and internal field
    fixedNormalSlipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedNormalSlipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    fixedNormalSlipPointPatchField
    (
      const fixedNormalSlipPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new fixedNormalSlipPointPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    fixedNormalSlipPointPatchField
    (
      const fixedNormalSlipPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type>> clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new fixedNormalSlipPointPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    // Evaluation functions
      //- Update the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "fixed_normal_slip_point_patch_field.ipp"

#endif

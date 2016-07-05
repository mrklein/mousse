#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_COUPLED_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_COUPLED_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledPointPatchField
// Description
//   A Coupled boundary condition for pointField

#include "point_patch_field.hpp"
#include "coupled_point_patch.hpp"


namespace mousse {

template<class Type>
class coupledPointPatchField
:
  public pointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(coupledPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    coupledPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    coupledPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary& dict
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    coupledPointPatchField
    (
      const coupledPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const = 0;
    //- Construct as copy setting internal field reference
    coupledPointPatchField
    (
      const coupledPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type>> clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const = 0;
  // Member functions
    // Access
      //- Return true if this patch field is derived from
      //  coupledFvPatchField<Type>.
      virtual bool coupled() const
      {
        return true;
      }
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      ) = 0;
      //- Initialise swap of patch point values
      virtual void initSwapAddSeparated
      (
        const Pstream::commsTypes,
        Field<Type>&
      ) const
      {}
      //- Complete swap of patch point values and add to local values
      virtual void swapAddSeparated
      (
        const Pstream::commsTypes,
        Field<Type>&
      ) const = 0;
};
}  // namespace mousse

#include "coupled_point_patch_field.ipp"

#endif

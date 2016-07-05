#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_CYCLIC_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_CYCLIC_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicPointPatchField
// Description
//   Cyclic front and back plane patch field

#include "coupled_point_patch_field.hpp"
#include "cyclic_point_patch.hpp"


namespace mousse {

template<class Type>
class cyclicPointPatchField
:
  public coupledPointPatchField<Type>
{
  // Private data
    //- Local reference cast into the cyclic patch
    const cyclicPointPatch& cyclicPatch_;
public:
  //- Runtime type information
  TYPE_NAME(cyclicPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    cyclicPointPatchField
    (
      const cyclicPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const
    {
      return autoPtr<pointPatchField<Type>>
      {
        new cyclicPointPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    cyclicPointPatchField
    (
      const cyclicPointPatchField<Type>&,
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
        new cyclicPointPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    //- Constraint handling
      //- Return the constraint type this pointPatchField implements
      virtual const word& constraintType() const
      {
        return cyclicPointPatch::typeName;
      }
    //- Cyclic coupled interface functions
      //- Does the patch field perform the transfromation
      virtual bool doTransform() const
      {
        return !(cyclicPatch_.parallel() || pTraits<Type>::rank == 0);
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicPatch_.reverseT();
      }
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes /*commsType*/=Pstream::blocking
      )
      {}
      //- Complete swap of patch point values and add to local values
      virtual void swapAddSeparated
      (
        const Pstream::commsTypes commsType,
        Field<Type>&
      ) const;
};

}  // namespace mousse

#include "cyclic_point_patch_field.ipp"

#endif

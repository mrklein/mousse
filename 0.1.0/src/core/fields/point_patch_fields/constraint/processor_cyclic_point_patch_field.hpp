#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_PROCESSOR_CYCLIC_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_CONSTRAINT_PROCESSOR_CYCLIC_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicPointPatchField
// Description
//   mousse::processorCyclicPointPatchField
// SourceFiles
//   processor_cyclic_point_patch_field.cpp
#include "coupled_point_patch_field.hpp"
#include "processor_cyclic_point_patch.hpp"
namespace mousse
{
template<class Type>
class processorCyclicPointPatchField
:
  public coupledPointPatchField<Type>
{
  // Private data
    //- Local reference to processor patch
    const processorCyclicPointPatch& procPatch_;
    //- Receive buffer for non-blocking communication
    mutable Field<Type> receiveBuf_;
public:
  //- Runtime type information
  TYPE_NAME(processorCyclicPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    processorCyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    processorCyclicPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    processorCyclicPointPatchField
    (
      const processorCyclicPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new processorCyclicPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    processorCyclicPointPatchField
    (
      const processorCyclicPointPatchField<Type>&,
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
        new processorCyclicPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  //- Destructor
  virtual ~processorCyclicPointPatchField();
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
      //- Does the patch field perform the transfromation
      virtual bool doTransform() const
      {
        return
         !(
            pTraits<Type>::rank == 0
          || procPatch_.procPolyPatch().parallel()
          );
      }
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes=Pstream::blocking
      )
      {}
      //- Initialise swap of non-collocated patch point values
      virtual void initSwapAddSeparated
      (
        const Pstream::commsTypes commsType,
        Field<Type>&
      ) const;
      //- Complete swap of patch point values and add to local values
      virtual void swapAddSeparated
      (
        const Pstream::commsTypes commsType,
        Field<Type>&
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#    include "processor_cyclic_point_patch_field.cpp"
#endif
#endif

#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_PROCESSOR_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_PROCESSOR_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition enables processor communication across patches.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            processor;
//   }
//   \endverbatim

#include "coupled_fv_patch_field.hpp"
#include "processor_ldu_interface_field.hpp"
#include "processor_fv_patch.hpp"


namespace mousse {

template<class Type>
class processorFvPatchField
:
  public processorLduInterfaceField,
  public coupledFvPatchField<Type>
{
  // Private data
    //- Local reference cast into the processor patch
    const processorFvPatch& procPatch_;
    // Sending and receiving
      //- Send buffer.
      mutable Field<Type> sendBuf_;
      //- Receive buffer.
      mutable Field<Type> receiveBuf_;
      //- Outstanding request
      mutable label outstandingSendRequest_;
      //- Outstanding request
      mutable label outstandingRecvRequest_;
      //- Scalar send buffer
      mutable Field<scalar> scalarSendBuf_;
      //- Scalar receive buffer
      mutable Field<scalar> scalarReceiveBuf_;
public:
  //- Runtime type information
  TYPE_NAME(processorFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    processorFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch and internal field and patch field
    processorFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    processorFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given processorFvPatchField onto a new patch
    processorFvPatchField
    (
      const processorFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    processorFvPatchField(const processorFvPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new processorFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    processorFvPatchField
    (
      const processorFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new processorFvPatchField<Type>{*this, iF}
      };
    }
  //- Destructor
  ~processorFvPatchField();
  // Member functions
    // Access
      //- Return true if running parallel
      virtual bool coupled() const
      {
        if (Pstream::parRun()) {
          return true;
        } else {
          return false;
        }
      }
      //- Return neighbour field given internal field
      virtual tmp<Field<Type>> patchNeighbourField() const;
    // Evaluation functions
      //- Initialise the evaluation of the patch field
      virtual void initEvaluate(const Pstream::commsTypes commsType);
      //- Evaluate the patch field
      virtual void evaluate(const Pstream::commsTypes commsType);
      //- Return patch-normal gradient
      virtual tmp<Field<Type>> snGrad
      (
        const scalarField& deltaCoeffs
      ) const;
      //- Is all data available
      virtual bool ready() const;
      //- Initialise neighbour matrix update
      virtual void initInterfaceMatrixUpdate
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
      //- Initialise neighbour matrix update
      virtual void initInterfaceMatrixUpdate
      (
        Field<Type>& result,
        const Field<Type>& psiInternal,
        const scalarField& coeffs,
        const Pstream::commsTypes commsType
      ) const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<Type>& result,
        const Field<Type>& psiInternal,
        const scalarField& coeffs,
        const Pstream::commsTypes commsType
      ) const;
    //- Processor coupled interface functions
      //- Return communicator used for comms
      virtual label comm() const
      {
        return procPatch_.comm();
      }
      //- Return processor number
      virtual int myProcNo() const
      {
        return procPatch_.myProcNo();
      }
      //- Return neigbour processor number
      virtual int neighbProcNo() const
      {
        return procPatch_.neighbProcNo();
      }
      //- Does the patch field perform the transfromation
      virtual bool doTransform() const
      {
        return !(procPatch_.parallel() || pTraits<Type>::rank == 0);
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return procPatch_.forwardT();
      }
      //- Return rank of component for transform
      virtual int rank() const
      {
        return pTraits<Type>::rank;
      }
};
}  // namespace mousse

#include "processor_fv_patch_field.ipp"

#endif

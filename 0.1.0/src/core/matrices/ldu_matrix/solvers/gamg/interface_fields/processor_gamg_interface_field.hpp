#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_PROCESSOR_GAMG_INTERFACE_FIELD_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_PROCESSOR_GAMG_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorGAMGInterfaceField
// Description
//   GAMG agglomerated processor interface field.

#include "gamg_interface_field.hpp"
#include "processor_gamg_interface.hpp"
#include "processor_ldu_interface_field.hpp"


namespace mousse {

class processorGAMGInterfaceField
:
  public GAMGInterfaceField,
  public processorLduInterfaceField
{
  // Private data

    //- Local reference cast into the processor interface
    const processorGAMGInterface& procInterface_;

    //- Is the transform required
    bool doTransform_;

    //- Rank of component for transformation
    int rank_;

    // Sending and receiving

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
  TYPE_NAME("processor");

  // Constructors

    //- Construct from GAMG interface and fine level interface field
    processorGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterface
    );

    //- Construct from GAMG interface and fine level interface field
    processorGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );

    //- Disallow default bitwise copy construct
    processorGAMGInterfaceField(const processorGAMGInterfaceField&) = delete;

    //- Disallow default bitwise assignment
    processorGAMGInterfaceField& operator=
    (
      const processorGAMGInterfaceField&
    ) = delete;

  //- Destructor
  virtual ~processorGAMGInterfaceField();

  // Member Functions

    // Access

      //- Return size
      label size() const
      {
        return procInterface_.size();
      }

    // Interface matrix update

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

    //- Processor interface functions

      //- Return communicator used for comms
      virtual label comm() const
      {
        return procInterface_.comm();
      }

      //- Return processor number
      virtual int myProcNo() const
      {
        return procInterface_.myProcNo();
      }

      //- Return neigbour processor number
      virtual int neighbProcNo() const
      {
        return procInterface_.neighbProcNo();
      }

      //- Does the interface field perform the transfromation
      virtual bool doTransform() const
      {
        return doTransform_;
      }

      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return procInterface_.forwardT();
      }

      //- Return rank of component for transform
      virtual int rank() const
      {
        return rank_;
      }

};

}  // namespace mousse

#endif

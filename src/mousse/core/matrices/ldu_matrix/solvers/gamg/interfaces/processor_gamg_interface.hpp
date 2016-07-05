#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_PROCESSOR_GAMG_INTERFACE_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_PROCESSOR_GAMG_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorGAMGInterface
// Description
//   GAMG agglomerated processor interface.

#include "gamg_interface.hpp"
#include "processor_ldu_interface.hpp"


namespace mousse {

class processorGAMGInterface
:
  public GAMGInterface,
  public processorLduInterface
{
  // Private data

    //- Communicator to use for parallel communication
    const label comm_;

    //- My processor rank in communicator
    label myProcNo_;

    //- Neighbouring processor rank in communicator
    label neighbProcNo_;

    //- Transformation tensor
    tensorField forwardT_;

    //- Message tag used for sending
    int tag_;

public:

  //- Runtime type information
  TYPE_NAME("processor");

  // Constructors

    //- Construct from fine-level interface,
    //  local and neighbour restrict addressing
    processorGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );

    //- Construct from components
    processorGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const labelUList& faceCells,
      const labelUList& faceRestrictAddresssing,
      const label coarseComm,
      const label myProcNo,
      const label neighbProcNo,
      const tensorField& forwardT,
      const int tag
    );

    //- Construct from Istream
    processorGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      Istream& is
    );

    //- Disallow default bitwise copy construct
    processorGAMGInterface(const processorGAMGInterface&) = delete;

    //- Disallow default bitwise assignment
    processorGAMGInterface& operator=(const processorGAMGInterface&) = delete;

  //- Destructor
  virtual ~processorGAMGInterface();

  // Member Functions

    // Interface transfer functions

      //- Initialise neighbour field transfer
      virtual void initInternalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;

      //- Transfer and return internal field adjacent to the interface
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;

    //- Processor interface functions

      //- Return communicator used for sending
      virtual label comm() const
      {
        return comm_;
      }

      //- Return processor number (rank in communicator)
      virtual int myProcNo() const
      {
        return myProcNo_;
      }

      //- Return neigbour processor number (rank in communicator)
      virtual int neighbProcNo() const
      {
        return neighbProcNo_;
      }

      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return forwardT_;
      }

      //- Return message tag used for sending
      virtual int tag() const
      {
        return tag_;
      }

    // I/O

      //- Write to stream
      virtual void write(Ostream&) const;

};

}  // namespace mousse
#endif

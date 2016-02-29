#ifndef CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_PROCESSOR_LDU_INTERFACE_HPP_
#define CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_PROCESSOR_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorLduInterface
// Description
//   An abstract base class for processor coupled interfaces.
// SourceFiles
//   processor_ldu_interface.cpp
//   processor_ldu_interface_templates.cpp


#include "ldu_interface.hpp"
#include "primitive_fields_fwd.hpp"

namespace mousse
{

class processorLduInterface
{
  // Private data

    //- Send buffer.
    //  Only sized and used when compressed or non-blocking comms used.
    mutable List<char> sendBuf_;

    //- Receive buffer.
    //  Only sized and used when compressed or non-blocking comms used.
    mutable List<char> receiveBuf_;

    //- Resize the buffer if required
    void resizeBuf(List<char>& buf, const label size) const;

public:
  //- Runtime type information
  TYPE_NAME("processorLduInterface");

  // Constructors

    //- Construct null
    processorLduInterface();

  //- Destructor
  virtual ~processorLduInterface();

  // Member Functions

    // Access

      //- Return communicator used for parallel communication
      virtual label comm() const = 0;

      //- Return processor number (rank in communicator)
      virtual int myProcNo() const = 0;

      //- Return neigbour processor number (rank in communicator)
      virtual int neighbProcNo() const = 0;

      //- Return face transformation tensor
      virtual const tensorField& forwardT() const = 0;

      //- Return message tag used for sending
      virtual int tag() const = 0;

    // Transfer functions

      //- Raw send function
      template<class Type>
      void send
      (
        const Pstream::commsTypes commsType,
        const UList<Type>&
      ) const;

      //- Raw field receive function
      template<class Type>
      void receive
      (
        const Pstream::commsTypes commsType,
        UList<Type>&
      ) const;

      //- Raw field receive function returning field
      template<class Type>
      tmp<Field<Type> > receive
      (
        const Pstream::commsTypes commsType,
        const label size
      ) const;

      //- Raw field send function with data compression
      template<class Type>
      void compressedSend
      (
        const Pstream::commsTypes commsType,
        const UList<Type>&
      ) const;

      //- Raw field receive function with data compression
      template<class Type>
      void compressedReceive
      (
        const Pstream::commsTypes commsType,
        UList<Type>&
      ) const;

      //- Raw field receive function with data compression returning field
      template<class Type>
      tmp<Field<Type> > compressedReceive
      (
        const Pstream::commsTypes commsType,
        const label size
      ) const;

};

}  // namespace mousse
#ifdef NoRepository
#   include "processor_ldu_interface_templates.cpp"
#endif
#endif

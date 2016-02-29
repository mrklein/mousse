#ifndef CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_LDU_INTERFACE_HPP_
#define CORE_MATRICES_LDU_MATRIX_LDU_ADDRESSING_LDU_INTERFACE_LDU_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lduInterface
// Description
//   An abstract base class for implicitly-coupled interfaces
//   e.g. processor and cyclic patches.
// SourceFiles
//   ldu_interface.cpp


#include "label_field.hpp"
#include "type_info.hpp"
#include "pstream.hpp"

namespace mousse
{

class lduInterface
{
public:

  //- Runtime type information
  TYPE_NAME("lduInterface");

  // Constructors

    //- Construct null
    lduInterface()
    {}

    //- Disallow default bitwise copy construct
    lduInterface(const lduInterface&) = delete;

    //- Disallow default bitwise assignment
    lduInterface& operator=(const lduInterface&) = delete;

  //- Destructor
  virtual ~lduInterface();

  // Member Functions

    // Access

      //- Return faceCell addressing
      virtual const labelUList& faceCells() const = 0;

    // Interface transfer functions

      //- Return the values of the given internal data adjacent to
      //  the interface as a field
      virtual tmp<labelField> interfaceInternalField
      (
        const labelUList& internalData
      ) const = 0;

      //- Initialise transfer of internal field adjacent to the interface
      virtual void initInternalFieldTransfer
      (
        const Pstream::commsTypes,
        const labelUList& /*iF*/
      ) const
      {}

      //- Transfer and return internal field adjacent to the interface
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const = 0;

};

}  // namespace mousse
#endif

#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_PROCESSOR_CYCLIC_GAMG_INTERFACE_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_PROCESSOR_CYCLIC_GAMG_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicGAMGInterface
// Description
//   GAMG agglomerated processor interface.
// SourceFiles
//   processor_cyclic_gamg_interface.cpp
//   processor_cyclic_gamg_interface_templates.cpp


#include "processor_gamg_interface.hpp"

namespace mousse
{

class processorCyclicGAMGInterface
:
  public processorGAMGInterface
{
public:

  //- Runtime type information
  TYPE_NAME("processorCyclic");

  // Constructors

    //- Construct from fine-level interface,
    //  local and neighbour restrict addressing
    processorCyclicGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );

    //- Construct from Istream
    processorCyclicGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      Istream& is
    );

    //- Disallow default bitwise copy construct
    processorCyclicGAMGInterface(const processorCyclicGAMGInterface&) = delete;

    //- Disallow default bitwise assignment
    processorCyclicGAMGInterface& operator=
    (
      const processorCyclicGAMGInterface&
    ) = delete;

  // Destructor
    virtual ~processorCyclicGAMGInterface();

};

}  // namespace mousse
#endif

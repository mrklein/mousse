#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_PROCESSOR_CYCLIC_GAMG_INTERFACE_FIELD_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACE_FIELDS_PROCESSOR_CYCLIC_GAMG_INTERFACE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicGAMGInterfaceField
// Description
//   GAMG agglomerated processor interface field.
// SourceFiles
//   processor_cyclic_gamg_interface_field.cpp


#include "processor_gamg_interface_field.hpp"

namespace mousse
{

class processorCyclicGAMGInterfaceField
:
  public processorGAMGInterfaceField
{

public:

  //- Runtime type information
  TYPE_NAME("processorCyclic");

  // Constructors

    //- Construct from GAMG interface and fine level interface field
    processorCyclicGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const lduInterfaceField& fineInterface
    );

    //- Construct from GAMG interface and fine level interface field
    processorCyclicGAMGInterfaceField
    (
      const GAMGInterface& GAMGCp,
      const bool doTransform,
      const int rank
    );

    //- Disallow default bitwise copy construct
    processorCyclicGAMGInterfaceField
    (
      const processorCyclicGAMGInterfaceField&
    ) = delete;

    //- Disallow default bitwise assignment
    processorCyclicGAMGInterfaceField& operator=
    (
      const processorCyclicGAMGInterfaceField&
    ) = delete;

  // Destructor
    virtual ~processorCyclicGAMGInterfaceField();

};

}  // namespace mousse
#endif

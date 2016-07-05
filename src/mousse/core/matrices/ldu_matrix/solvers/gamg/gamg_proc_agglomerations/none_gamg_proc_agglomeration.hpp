#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_NONE_GAMG_PROC_AGGLOMERATION_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_NONE_GAMG_PROC_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noneGAMGProcAgglomeration
// Description
//   Processor agglomeration of GAMGAgglomerations.

#include "gamg_proc_agglomeration.hpp"
#include "dynamic_list.hpp"


namespace mousse {

class GAMGAgglomeration;

class noneGAMGProcAgglomeration
:
  public GAMGProcAgglomeration
{
public:

  //- Runtime type information
  TYPE_NAME("noneGAMGProcAgglomeration");

  // Constructors

    //- Construct given agglomerator and controls
    noneGAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    noneGAMGProcAgglomeration
    (
      const noneGAMGProcAgglomeration&
    ) = delete;

    //- Disallow default bitwise assignment
    noneGAMGProcAgglomeration& operator=
    (
      const noneGAMGProcAgglomeration&
    ) = delete;

  //- Destructor
  virtual ~noneGAMGProcAgglomeration();

  // Member Functions

   //- Modify agglomeration. Return true if modified
    virtual bool agglomerate()
    {
      return false;
    }
};

}  // namespace mousse
#endif

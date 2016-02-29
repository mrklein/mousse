#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_EAGER_GAMG_PROC_AGGLOMERATION_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_PROC_AGGLOMERATIONS_EAGER_GAMG_PROC_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::eagerGAMGProcAgglomeration
// Description
//   'Eager' processor agglomeration of GAMGAgglomerations: at every
//   level agglomerates 'mergeLevels' number of processors onto the
//   minimum processor number.
// SourceFiles
//   eager_gamg_proc_agglomeration.cpp


#include "gamg_proc_agglomeration.hpp"
#include "dynamic_list.hpp"

namespace mousse
{
class GAMGAgglomeration;
class eagerGAMGProcAgglomeration
:
  public GAMGProcAgglomeration
{
  // Private data

    //- Agglpmeration level
    const label mergeLevels_;

    DynamicList<label> comms_;

public:

  //- Runtime type information
  TYPE_NAME("eager");

  // Constructors

    //- Construct given agglomerator and controls
    eagerGAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    eagerGAMGProcAgglomeration
    (
      const eagerGAMGProcAgglomeration&
    ) = delete;

    //- Disallow default bitwise assignment
    eagerGAMGProcAgglomeration& operator=
    (
      const eagerGAMGProcAgglomeration&
    ) = delete;

  //- Destructor
  virtual ~eagerGAMGProcAgglomeration();

  // Member Functions

   //- Modify agglomeration. Return true if modified
    virtual bool agglomerate();
};

}  // namespace mousse

#endif

#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_AGGLOMERATIONS_ALGEBRAIC_PAIR_GAMG_AGGLOMERATION_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_GAMG_AGGLOMERATIONS_ALGEBRAIC_PAIR_GAMG_AGGLOMERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::algebraicPairGAMGAgglomeration
// Description
//   Agglomerate using the pair algorithm.

#include "pair_gamg_agglomeration.hpp"


namespace mousse {

class algebraicPairGAMGAgglomeration
:
  public pairGAMGAgglomeration
{
public:
  //- Runtime type information
  TYPE_NAME("algebraicPair");
  // Constructors
    //- Construct given mesh and controls
    algebraicPairGAMGAgglomeration
    (
      const lduMatrix& matrix,
      const dictionary& controlDict
    );
};

}  // namespace mousse

#endif

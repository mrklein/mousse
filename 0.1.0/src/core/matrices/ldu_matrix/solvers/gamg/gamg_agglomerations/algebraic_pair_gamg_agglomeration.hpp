// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::algebraicPairGAMGAgglomeration
// Description
//   Agglomerate using the pair algorithm.
// SourceFiles
//   algebraic_pair_gamg_agglomeration.cpp

#ifndef algebraic_pair_gamg_agglomeration_hpp_
#define algebraic_pair_gamg_agglomeration_hpp_

#include "pair_gamg_agglomeration.hpp"

namespace mousse
{
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

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "algebraic_pair_gamg_agglomeration.hpp"
#include "ldu_matrix.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(algebraicPairGAMGAgglomeration, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGAgglomeration,
  algebraicPairGAMGAgglomeration,
  lduMatrix
);

}


// Constructors 
mousse::algebraicPairGAMGAgglomeration::algebraicPairGAMGAgglomeration
(
  const lduMatrix& matrix,
  const dictionary& controlDict
)
:
  pairGAMGAgglomeration{matrix.mesh(), controlDict}
{
  agglomerate(matrix.mesh(), mag(matrix.upper()));
}

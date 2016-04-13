// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_gamg_agglomeration.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pairGAMGAgglomeration, 0);
bool pairGAMGAgglomeration::forward_{true};

}


// Constructors 
mousse::pairGAMGAgglomeration::pairGAMGAgglomeration
(
  const lduMesh& mesh,
  const dictionary& controlDict
)
:
  GAMGAgglomeration{mesh, controlDict},
  mergeLevels_{controlDict.lookupOrDefault<label>("mergeLevels", 1)}
{}

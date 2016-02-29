// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "antal.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace wallLubricationModels {
DEFINE_TYPE_NAME_AND_DEBUG(Antal, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  wallLubricationModel,
  Antal,
  dictionary
);
}
}
// Constructors 
mousse::wallLubricationModels::Antal::Antal
(
  const dictionary& dict,
  const phasePair& pair
)
:
  wallLubricationModel{dict, pair},
  Cw1_{"Cw1", dimless, dict},
  Cw2_{"Cw2", dimless, dict}
{}
// Destructor 
mousse::wallLubricationModels::Antal::~Antal()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField> mousse::wallLubricationModels::Antal::Fi() const
{
  volVectorField Ur{pair_.Ur()};
  const volVectorField& n = nWall();
  return
    max(dimensionedScalar{"zero", dimless/dimLength, 0},
        Cw1_/pair_.dispersed().d() + Cw2_/yWall())
    *pair_.continuous().rho()*magSqr(Ur - (Ur & n)*n)*n;
}

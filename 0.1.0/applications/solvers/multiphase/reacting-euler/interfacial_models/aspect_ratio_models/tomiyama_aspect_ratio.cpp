// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_aspect_ratio.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace aspectRatioModels {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaAspectRatio, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  aspectRatioModel,
  TomiyamaAspectRatio,
  dictionary
);
}
}
// Constructors 
mousse::aspectRatioModels::TomiyamaAspectRatio::TomiyamaAspectRatio
(
  const dictionary& dict,
  const phasePair& pair
)
:
  VakhrushevEfremov{dict, pair},
  wallDependentModel{pair.phase1().mesh()}
{}
// Destructor 
mousse::aspectRatioModels::TomiyamaAspectRatio::~TomiyamaAspectRatio()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::aspectRatioModels::TomiyamaAspectRatio::E() const
{
  return
    VakhrushevEfremov::E()*max(scalar(1) - 0.35*yWall()/pair_.dispersed().d(),
                               scalar(0.65));
}

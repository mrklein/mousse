// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_correlated.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaCorrelated, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, TomiyamaCorrelated, dictionary);
}
}
// Constructors 
mousse::dragModels::TomiyamaCorrelated::TomiyamaCorrelated
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject},
  A_{"A", dimless, dict}
{}
// Destructor 
mousse::dragModels::TomiyamaCorrelated::~TomiyamaCorrelated()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::dragModels::TomiyamaCorrelated::CdRe() const
{
  volScalarField Re{pair_.Re()};
  volScalarField Eo{pair_.Eo()};
  return
    max(A_*min((1 + 0.15*pow(Re, 0.687)),
               scalar(3)),
        8*Eo*Re/(3*Eo + 12));
}

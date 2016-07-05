// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_analytic.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaAnalytic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, TomiyamaAnalytic, dictionary);
}
}
// Constructors 
mousse::dragModels::TomiyamaAnalytic::TomiyamaAnalytic
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject},
  residualRe_{"residualRe", dimless, dict},
  residualEo_{"residualEo", dimless, dict},
  residualE_{"residualE", dimless, dict}
{}
// Destructor 
mousse::dragModels::TomiyamaAnalytic::~TomiyamaAnalytic()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::dragModels::TomiyamaAnalytic::CdRe() const
{
  volScalarField Eo{max(pair_.Eo(), residualEo_)};
  volScalarField E{max(pair_.E(), residualE_)};
  volScalarField OmEsq{max(scalar(1) - sqr(E), sqr(residualE_))};
  volScalarField rtOmEsq{sqrt(OmEsq)};
  volScalarField F{max(asin(rtOmEsq) - E*rtOmEsq, residualE_)/OmEsq};
  return
    (8.0/3.0)*Eo/(Eo*pow(E, 2.0/3.0)/OmEsq
                  + 16*pow(E, 4.0/3.0))/sqr(F)*max(pair_.Re(), residualRe_);
}

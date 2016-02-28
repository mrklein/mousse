// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gidaspow_schiller_naumann.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(GidaspowSchillerNaumann, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, GidaspowSchillerNaumann, dictionary);
}
}
// Constructors 
mousse::dragModels::GidaspowSchillerNaumann::GidaspowSchillerNaumann
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject},
  residualRe_{"residualRe", dimless, dict}
{}
// Destructor 
mousse::dragModels::GidaspowSchillerNaumann::~GidaspowSchillerNaumann()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::dragModels::GidaspowSchillerNaumann::CdRe() const
{
  volScalarField alpha2
  {
    max(scalar(1) - pair_.dispersed(), pair_.continuous().residualAlpha())
  };
  volScalarField Re{alpha2*pair_.Re()};
  volScalarField CdsRe
  {
    neg(Re - 1000)*24.0*(1.0 + 0.15*pow(Re, 0.687))/alpha2
    + pos(Re - 1000)*0.44*max(Re, residualRe_)
  };
  return CdsRe*pow(alpha2, -2.65)
    *max(pair_.continuous(), pair_.continuous().residualAlpha());
}

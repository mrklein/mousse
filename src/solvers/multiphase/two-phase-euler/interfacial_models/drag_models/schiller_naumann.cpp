// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "schiller_naumann.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(SchillerNaumann, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, SchillerNaumann, dictionary);
}
}
// Constructors 
mousse::dragModels::SchillerNaumann::SchillerNaumann
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
mousse::dragModels::SchillerNaumann::~SchillerNaumann()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::SchillerNaumann::CdRe() const
{
  volScalarField Re{pair_.Re()};
  return neg(Re - 1000)*24.0*(1.0 + 0.15*pow(Re, 0.687))
    + pos(Re - 1000)*0.44*max(Re, residualRe_);
}

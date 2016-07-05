// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lain.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(Lain, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, Lain, dictionary);
}
}
// Constructors 
mousse::dragModels::Lain::Lain
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject}
{}
// Destructor 
mousse::dragModels::Lain::~Lain()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::Lain::CdRe() const
{
  volScalarField Re{pair_.Re()};
  return
    neg(Re - 1.5)*16.0
    + pos(Re - 1.5)*neg(Re - 80.0)*14.9*pow(Re, 0.22)
    + pos(Re - 80.0)*neg(Re - 1500.0)*48*(1.0 - 2.21/sqrt(max(Re, SMALL)))
    + pos(Re - 1500.0)*2.61*Re;
}

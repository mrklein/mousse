// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wen_yu.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(WenYu, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, WenYu, dictionary);
}
}
// Constructors 
mousse::dragModels::WenYu::WenYu
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
mousse::dragModels::WenYu::~WenYu()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::WenYu::CdRe() const
{
  volScalarField alpha2
  {
    max(scalar(1) - pair_.dispersed(), pair_.continuous().residualAlpha())
  };
  volScalarField Res{alpha2*pair_.Re()};
  volScalarField CdsRes
  {
    neg(Res - 1000)*24.0*(1.0 + 0.15*pow(Res, 0.687))
    + pos(Res - 1000)*0.44*max(Res, residualRe_)
  };
  return
    CdsRes*pow(alpha2, -3.65)
    *max(pair_.continuous(), pair_.continuous().residualAlpha());
}

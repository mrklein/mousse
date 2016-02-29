// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ishii_zuber.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(IshiiZuber, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, IshiiZuber, dictionary);
}
}
// Constructors 
mousse::dragModels::IshiiZuber::IshiiZuber
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject}
{}
// Destructor 
mousse::dragModels::IshiiZuber::~IshiiZuber()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::dragModels::IshiiZuber::CdRe() const
{
  volScalarField Re{pair_.Re()};
  volScalarField Eo{pair_.Eo()};
  volScalarField mud{pair_.dispersed().mu()};
  volScalarField muc{pair_.continuous().mu()};
  volScalarField muStar{(mud + 0.4*muc)/(mud + muc)};
  volScalarField muMix
  {
    muc*pow(max(1 - pair_.dispersed(), scalar(1e-3)), -2.5*muStar)
  };
  volScalarField ReM{Re*muc/muMix};
  volScalarField CdRe
  {
    pos(1000 - ReM)*24.0*(scalar(1) + 0.15*pow(ReM, 0.687))
    + neg(1000 - ReM)*0.44*ReM
  };
  volScalarField F{(muc/muMix)*sqrt(1 - pair_.dispersed())};
  F.max(1e-3);
  volScalarField Ealpha{(1 + 17.67*pow(F, 0.8571428))/(18.67*F)};
  volScalarField CdReEllipse{Ealpha*0.6666*sqrt(Eo)*Re};
  return pos(CdReEllipse - CdRe)
    *min(CdReEllipse, Re*sqr(1 - pair_.dispersed())*2.66667)
    + neg(CdReEllipse - CdRe)*CdRe;
}

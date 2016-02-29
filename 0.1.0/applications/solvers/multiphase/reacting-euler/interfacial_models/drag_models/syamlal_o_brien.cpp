// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "syamlal_o_brien.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(SyamlalOBrien, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, SyamlalOBrien, dictionary);
}
}
// Constructors 
mousse::dragModels::SyamlalOBrien::SyamlalOBrien
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject}
{}
// Destructor 
mousse::dragModels::SyamlalOBrien::~SyamlalOBrien()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::SyamlalOBrien::CdRe() const
{
  volScalarField alpha2
  {
    max(scalar(1) - pair_.dispersed(), pair_.continuous().residualAlpha())
  };
  volScalarField A{pow(alpha2, 4.14)};
  volScalarField B
  {
    neg(alpha2 - 0.85)*(0.8*pow(alpha2, 1.28))
    + pos(alpha2 - 0.85)*(pow(alpha2, 2.65))
  };
  volScalarField Re{pair_.Re()};
  volScalarField Vr
  {
    0.5*(A - 0.06*Re + sqrt(sqr(0.06*Re) + 0.12*Re*(2.0*B - A) + sqr(A)))
  };
  volScalarField CdsRe{sqr(0.63*sqrt(Re) + 4.8*sqrt(Vr))};
  return
    CdsRe
    *max(pair_.continuous(), pair_.continuous().residualAlpha())/sqr(Vr);
}

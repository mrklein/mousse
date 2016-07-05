// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "legendre_magnaudet.hpp"
#include "phase_pair.hpp"
#include "fvc_grad.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace liftModels {
DEFINE_TYPE_NAME_AND_DEBUG(LegendreMagnaudet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liftModel, LegendreMagnaudet, dictionary);
}
}
// Constructors 
mousse::liftModels::LegendreMagnaudet::LegendreMagnaudet
(
  const dictionary& dict,
  const phasePair& pair
)
:
  liftModel{dict, pair},
  residualRe_{"residualRe", dimless, dict}
{}
// Destructor 
mousse::liftModels::LegendreMagnaudet::~LegendreMagnaudet()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::liftModels::LegendreMagnaudet::Cl() const
{
  volScalarField Re{max(pair_.Re(), residualRe_)};
  volScalarField Sr
  {
    sqr(pair_.dispersed().d())/(Re*pair_.continuous().nu())
    *mag(fvc::grad(pair_.continuous().U()))
  };
  volScalarField ClLowSqr
  {
    sqr(6.0*2.255)*sqr(Sr)/(pow4(constant::mathematical::pi)*Re*pow3(Sr + 0.2*Re))
  };
  volScalarField ClHighSqr
  {
    sqr(0.5*(Re + 16.0)/(Re + 29.0))
  };
  return sqrt(ClLowSqr + ClHighSqr);
}

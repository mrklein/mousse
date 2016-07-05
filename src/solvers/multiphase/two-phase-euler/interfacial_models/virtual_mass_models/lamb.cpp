// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lamb.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace virtualMassModels {
DEFINE_TYPE_NAME_AND_DEBUG(Lamb, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  virtualMassModel,
  Lamb,
  dictionary
);
}
}
// Constructors 
mousse::virtualMassModels::Lamb::Lamb
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  virtualMassModel{dict, pair, registerObject}
{}
// Destructor 
mousse::virtualMassModels::Lamb::~Lamb()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::virtualMassModels::Lamb::Cvm() const
{
  volScalarField E{min(max(pair_.E(), SMALL), 1 - SMALL)};
  volScalarField rtOmEsq{sqrt(1 - sqr(E))};
  return (rtOmEsq - E*acos(E))/(E*acos(E) - sqr(E)*rtOmEsq);
}

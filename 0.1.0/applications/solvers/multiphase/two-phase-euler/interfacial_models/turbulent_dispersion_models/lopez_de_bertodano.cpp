// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lopez_de_bertodano.hpp"
#include "phase_pair.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace turbulentDispersionModels {
DEFINE_TYPE_NAME_AND_DEBUG(LopezDeBertodano, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  turbulentDispersionModel,
  LopezDeBertodano,
  dictionary
);
}
}
// Constructors 
mousse::turbulentDispersionModels::LopezDeBertodano::LopezDeBertodano
(
  const dictionary& dict,
  const phasePair& pair
)
:
  turbulentDispersionModel{dict, pair},
  Ctd_{"Ctd", dimless, dict}
{}
// Destructor 
mousse::turbulentDispersionModels::LopezDeBertodano::~LopezDeBertodano()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::turbulentDispersionModels::LopezDeBertodano::D() const
{
  return
    Ctd_
    *pair_.continuous().rho()
    *pair_.continuous().turbulence().k();
}

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "burns.hpp"
#include "phase_pair.hpp"
#include "euler_phase_compressible_turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "drag_model.hpp"
// Static Data Members
namespace mousse {
namespace turbulentDispersionModels {
DEFINE_TYPE_NAME_AND_DEBUG(Burns, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  turbulentDispersionModel,
  Burns,
  dictionary
);
}
}
// Constructors 
mousse::turbulentDispersionModels::Burns::Burns
(
  const dictionary& dict,
  const phasePair& pair
)
:
  turbulentDispersionModel{dict, pair},
  sigma_{"sigma", dimless, dict},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    dict.lookupOrDefault<scalar>
    (
      "residualAlpha",
      pair_.dispersed().residualAlpha().value()
    )
  }
{}
// Destructor 
mousse::turbulentDispersionModels::Burns::~Burns()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::turbulentDispersionModels::Burns::D() const
{
  const fvMesh& mesh = pair_.phase1().mesh();
  const dragModel& drag = mesh.lookupObject<dragModel>
    (
      IOobject::groupName(dragModel::typeName, pair_.name())
    );
  return
    0.75
    *drag.CdRe()
    *pair_.continuous().nu()
    *pair_.continuous().turbulence().nut()
    /(sigma_*sqr(pair_.dispersed().d()))
    *pair_.continuous().rho()
    *(1.0 + pair_.dispersed()/max(pair_.continuous(), residualAlpha_));
}

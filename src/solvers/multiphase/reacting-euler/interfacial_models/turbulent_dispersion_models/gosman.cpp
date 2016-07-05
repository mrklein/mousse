// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gosman.hpp"
#include "phase_pair.hpp"
#include "euler_phase_compressible_turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "drag_model.hpp"
// Static Data Members
namespace mousse {
namespace turbulentDispersionModels {
DEFINE_TYPE_NAME_AND_DEBUG(Gosman, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  turbulentDispersionModel,
  Gosman,
  dictionary
);
}
}
// Constructors 
mousse::turbulentDispersionModels::Gosman::Gosman
(
  const dictionary& dict,
  const phasePair& pair
)
:
  turbulentDispersionModel{dict, pair},
  sigma_{"sigma", dimless, dict}
{}
// Destructor 
mousse::turbulentDispersionModels::Gosman::~Gosman()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::turbulentDispersionModels::Gosman::D() const
{
  const fvMesh& mesh = pair_.phase1().mesh();
  const dragModel& drag = mesh.lookupObject<dragModel>
    (
      IOobject::groupName(dragModel::typeName, pair_.name())
    );
  return
    0.75
    *drag.CdRe()
    *pair_.dispersed()
    *pair_.continuous().nu()
    *pair_.continuous().turbulence().nut()
    /(sigma_*sqr(pair_.dispersed().d()))
    *pair_.continuous().rho();
}

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_lift_coefficient.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace liftModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantLiftCoefficient, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liftModel, constantLiftCoefficient, dictionary);
}
}
// Constructors 
mousse::liftModels::constantLiftCoefficient::constantLiftCoefficient
(
  const dictionary& dict,
  const phasePair& pair
)
:
  liftModel{dict, pair},
  Cl_{"Cl", dimless, dict}
{}
// Destructor 
mousse::liftModels::constantLiftCoefficient::~constantLiftCoefficient()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::liftModels::constantLiftCoefficient::Cl() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "zero",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        Cl_
      }
    };
}

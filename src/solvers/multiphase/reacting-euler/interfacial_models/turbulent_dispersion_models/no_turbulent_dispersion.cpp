// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_turbulent_dispersion.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace turbulentDispersionModels {
DEFINE_TYPE_NAME_AND_DEBUG(noTurbulentDispersion, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  turbulentDispersionModel,
  noTurbulentDispersion,
  dictionary
);
}
}
// Constructors 
mousse::turbulentDispersionModels::noTurbulentDispersion::noTurbulentDispersion
(
  const dictionary& dict,
  const phasePair& pair
)
:
  turbulentDispersionModel{dict, pair}
{}
// Destructor 
mousse::turbulentDispersionModels::noTurbulentDispersion::
~noTurbulentDispersion()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::turbulentDispersionModels::noTurbulentDispersion::D() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "zero",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      {"zero", dimD, 0}
    }
  };
}
mousse::tmp<mousse::volVectorField>
mousse::turbulentDispersionModels::noTurbulentDispersion::F() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return
    tmp<volVectorField>
    {
      new volVectorField
      {
        {
          "zero",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        {"zero", dimF, vector::zero}
      }
    };
}

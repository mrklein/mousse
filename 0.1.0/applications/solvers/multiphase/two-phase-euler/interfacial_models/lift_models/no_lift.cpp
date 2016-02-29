// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_lift.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace liftModels {
DEFINE_TYPE_NAME_AND_DEBUG(noLift, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liftModel, noLift, dictionary);
}
}
// Constructors 
mousse::liftModels::noLift::noLift
(
  const dictionary& dict,
  const phasePair& pair
)
:
  liftModel{dict, pair}
{}
// Destructor 
mousse::liftModels::noLift::~noLift()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::liftModels::noLift::Cl() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "Cl",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      {"Cl", dimless, 0}
    }
  };
}
mousse::tmp<mousse::volVectorField> mousse::liftModels::noLift::F() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volVectorField>
  {
    new volVectorField
    {
      {
        "noLift:F",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      {"zero", dimF, vector::zero}
    }
  };
}
mousse::tmp<mousse::surfaceScalarField> mousse::liftModels::noLift::Ff() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<surfaceScalarField>
  {
    new surfaceScalarField
    {
      {
        "noLift:Ff",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      {"zero", dimF*dimArea, 0}
    }
  };
}

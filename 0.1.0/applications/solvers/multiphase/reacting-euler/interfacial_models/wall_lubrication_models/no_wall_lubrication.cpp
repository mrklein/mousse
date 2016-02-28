// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_wall_lubrication.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace wallLubricationModels {
DEFINE_TYPE_NAME_AND_DEBUG(noWallLubrication, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  wallLubricationModel,
  noWallLubrication,
  dictionary
);
}
}
// Constructors 
mousse::wallLubricationModels::noWallLubrication::noWallLubrication
(
  const dictionary& dict,
  const phasePair& pair
)
:
  wallLubricationModel{dict, pair}
{}
// Destructor 
mousse::wallLubricationModels::noWallLubrication::~noWallLubrication()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField>
mousse::wallLubricationModels::noWallLubrication::Fi() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volVectorField>
  {
    new volVectorField
    {
      {
        "noWallLubrication:Fi",
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
mousse::tmp<mousse::volVectorField>
mousse::wallLubricationModels::noWallLubrication::F() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return tmp<volVectorField>
  {
    new volVectorField
    {
      {
        "noWallLubrication:F",
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

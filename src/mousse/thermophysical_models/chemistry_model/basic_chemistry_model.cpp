// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_chemistry_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(basicChemistryModel, 0);

}


// Protected Member Functions 
void mousse::basicChemistryModel::correct()
{
  // do nothing
}


// Constructors 
mousse::basicChemistryModel::basicChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  IOdictionary
  {
    {
      IOobject::groupName("chemistryProperties", phaseName),
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  },
  mesh_{mesh},
  chemistry_{lookup("chemistry")},
  deltaTChemIni_{readScalar(lookup("initialChemicalTimeStep"))},
  deltaTChem_
  {
    {
      IOobject::groupName("deltaTChem", phaseName),
      mesh.time().constant(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    {"deltaTChem0", dimTime, deltaTChemIni_}
  }
{}


// Destructor 
mousse::basicChemistryModel::~basicChemistryModel()
{}


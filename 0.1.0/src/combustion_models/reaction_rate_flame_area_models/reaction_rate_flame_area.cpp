// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction_rate_flame_area.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(reactionRateFlameArea, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(reactionRateFlameArea, dictionary);

}


// Constructors 
mousse::reactionRateFlameArea::reactionRateFlameArea
(
  const dictionary& dict,
  const fvMesh& mesh,
  const combustionModel& combModel
)
:
  coeffDict_{dictionary::null},
  mesh_{mesh},
  combModel_{combModel},
  fuel_{dict.lookup("fuel")},
  omega_
  {
    {
      "omega",
      mesh_.time().timeName(),
      mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh_
  }
{}


mousse::reactionRateFlameArea::reactionRateFlameArea
(
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh,
  const combustionModel& combModel
)
:
  coeffDict_{dict.subDict(modelType + "Coeffs")},
  mesh_{mesh},
  combModel_{combModel},
  fuel_{dict.lookup("fuel")},
  omega_
  {
    {
      "omega",
      mesh_.time().timeName(),
      mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh_
  }
{}


// Destructor 
mousse::reactionRateFlameArea::~reactionRateFlameArea()
{}


// Member Functions 
bool mousse::reactionRateFlameArea::read(const dictionary& dict)
{
  dict.lookup("fuel") >> fuel_;
  return true;
}


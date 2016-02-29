// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_tension_model.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(surfaceTensionModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(surfaceTensionModel, dictionary);
}
const mousse::dimensionSet
  mousse::surfaceTensionModel::dimSigma{1, 0, -2, 0, 0};
// Constructors 
mousse::surfaceTensionModel::surfaceTensionModel
(
  const dictionary& /*dict*/,
  const phasePair& pair,
  const bool registerObject
)
:
  regIOobject
  {
    {
      IOobject::groupName(typeName, pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      registerObject
    }
  },
  pair_{pair}
{}
// Destructor 
mousse::surfaceTensionModel::~surfaceTensionModel()
{}
// Member Functions 
bool mousse::surfaceTensionModel::writeData(Ostream& os) const
{
  return os.good();
}

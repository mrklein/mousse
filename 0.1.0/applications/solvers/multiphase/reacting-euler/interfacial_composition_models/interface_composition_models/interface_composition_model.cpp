// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interface_composition_model.hpp"
#include "phase_model.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(interfaceCompositionModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(interfaceCompositionModel, dictionary);
}
// Constructors 
mousse::interfaceCompositionModel::interfaceCompositionModel
(
  const dictionary& dict,
  const phasePair& pair
)
:
  pair_{pair},
  speciesNames_{dict.lookup("species")}
{}
// Destructor 
mousse::interfaceCompositionModel::~interfaceCompositionModel()
{}
// Member Functions 
const mousse::hashedWordList& mousse::interfaceCompositionModel::species() const
{
  return speciesNames_;
}
bool mousse::interfaceCompositionModel::transports
(
  word& speciesName
) const
{
  if (this->speciesNames_.contains(speciesName))
  {
    return true;
  }
  return false;
}

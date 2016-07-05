// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "force.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(force, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(force, dictionary);


// Constructors 
force::force(surfaceFilmModel& owner)
:
  filmSubModelBase{owner}
{}


force::force
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase{owner, dict, typeName, modelType}
{}


// Destructor 
force::~force()
{}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse


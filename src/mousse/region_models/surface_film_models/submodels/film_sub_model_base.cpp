// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_sub_model_base.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Constructors 
filmSubModelBase::filmSubModelBase(surfaceFilmModel& owner)
:
  subModelBase{owner.outputProperties()},
  owner_{owner}
{}


filmSubModelBase::filmSubModelBase
(
  surfaceFilmModel& owner,
  const dictionary& dict,
  const word& baseName,
  const word& modelType,
  const word& dictExt
)
:
  subModelBase
  {
    owner.outputProperties(),
    dict,
    baseName,
    modelType,
    dictExt
  },
  owner_{owner}
{}


filmSubModelBase::filmSubModelBase
(
  const word& modelName,
  surfaceFilmModel& owner,
  const dictionary& dict,
  const word& baseName,
  const word& modelType
)
:
  subModelBase
  {
    modelName,
    owner.outputProperties(),
    dict,
    baseName,
    modelType
  },
  owner_{owner}
{}


// Destructor 
filmSubModelBase::~filmSubModelBase()
{}


// Member Functions 
bool filmSubModelBase::outputTime() const
{
  return active() && owner_.time().outputTime();
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse


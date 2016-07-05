// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_turbulence_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(filmTurbulenceModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(filmTurbulenceModel, dictionary);


// Constructors 
filmTurbulenceModel::filmTurbulenceModel(surfaceFilmModel& owner)
:
  filmSubModelBase{owner}
{}


filmTurbulenceModel::filmTurbulenceModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase{owner, dict, typeName, modelType}
{}


// Destructor 
filmTurbulenceModel::~filmTurbulenceModel()
{}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse


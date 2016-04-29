// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_viscosity_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(filmViscosityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(filmViscosityModel, dictionary);


// Constructors 
filmViscosityModel::filmViscosityModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmSubModelBase{owner, dict, typeName, modelType},
  mu_{mu}
{}


// Destructor 
filmViscosityModel::~filmViscosityModel()
{}


// Member Functions 
void filmViscosityModel::info(Ostream&) const
{}

} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace mousse


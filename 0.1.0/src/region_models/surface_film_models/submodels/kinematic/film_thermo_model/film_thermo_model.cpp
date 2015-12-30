// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_thermo_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(filmThermoModel, 0);
defineRunTimeSelectionTable(filmThermoModel, dictionary);
// Constructors 
filmThermoModel::filmThermoModel
(
  surfaceFilmModel& owner
)
:
  filmSubModelBase(owner)
{}
filmThermoModel::filmThermoModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase(owner, dict, typeName, modelType)
{}
// Destructor 
filmThermoModel::~filmThermoModel()
{}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

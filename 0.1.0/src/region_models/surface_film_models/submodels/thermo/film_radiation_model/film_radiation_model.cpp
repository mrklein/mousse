// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_radiation_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(filmRadiationModel, 0);
defineRunTimeSelectionTable(filmRadiationModel, dictionary);
// Constructors 
filmRadiationModel::filmRadiationModel(surfaceFilmModel& owner)
:
  filmSubModelBase(owner)
{}
filmRadiationModel::filmRadiationModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmSubModelBase(owner, dict, typeName, modelType)
{}
// Destructor 
filmRadiationModel::~filmRadiationModel()
{}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

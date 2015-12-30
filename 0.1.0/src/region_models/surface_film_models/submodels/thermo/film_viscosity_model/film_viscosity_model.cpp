// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(filmViscosityModel, 0);
defineRunTimeSelectionTable(filmViscosityModel, dictionary);
// Constructors 
filmViscosityModel::filmViscosityModel
(
  const word& modelType,
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmSubModelBase(owner, dict, typeName, modelType),
  mu_(mu)
{}
// Destructor 
filmViscosityModel::~filmViscosityModel()
{}
// Member Functions 
void filmViscosityModel::info(Ostream& os) const
{}
} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace mousse

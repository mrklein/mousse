// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "liquid_viscosity.hpp"
#include "thermo_single_layer.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(liquidViscosity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmViscosityModel,
  liquidViscosity,
  dictionary
);
// Constructors 
liquidViscosity::liquidViscosity
(
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmViscosityModel(typeName, owner, dict, mu)
{}
// Destructor 
liquidViscosity::~liquidViscosity()
{}
// Member Functions 
void liquidViscosity::correct
(
  const volScalarField& /*p*/,
  const volScalarField& /*T*/
)
{
  const thermoSingleLayer& film = filmType<thermoSingleLayer>();
  mu_ = film.filmThermo().mu();
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

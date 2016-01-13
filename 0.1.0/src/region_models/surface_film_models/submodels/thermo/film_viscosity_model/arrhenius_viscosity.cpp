// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arrhenius_viscosity.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(ArrheniusViscosity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmViscosityModel,
  ArrheniusViscosity,
  dictionary
);
// Constructors 
ArrheniusViscosity::ArrheniusViscosity
(
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmViscosityModel(typeName, owner, dict, mu),
  viscosity_(filmViscosityModel::New(owner, coeffDict_, mu)),
  k1_("k1", dimTemperature, coeffDict_),
  k2_("k2", dimTemperature, coeffDict_),
  Tref_("Tref", dimTemperature, coeffDict_)
{}
// Destructor 
ArrheniusViscosity::~ArrheniusViscosity()
{}
// Member Functions 
void ArrheniusViscosity::correct
(
  const volScalarField& p,
  const volScalarField& T
)
{
  viscosity_->correct(p, T);
  mu_ *= exp(k1_*((1/(T + k2_)) - 1/(Tref_ + k2_)));
  mu_.correctBoundaryConditions();
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

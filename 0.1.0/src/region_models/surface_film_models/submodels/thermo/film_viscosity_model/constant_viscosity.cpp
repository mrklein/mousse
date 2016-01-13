// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_viscosity.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(constantViscosity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmViscosityModel,
  constantViscosity,
  dictionary
);
// Constructors 
constantViscosity::constantViscosity
(
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmViscosityModel(typeName, owner, dict, mu),
  mu0_("mu0", dimDynamicViscosity, coeffDict_)
{
  mu_ == mu0_;
}
// Destructor 
constantViscosity::~constantViscosity()
{}
// Member Functions 
void constantViscosity::correct
(
  const volScalarField& /*p*/,
  const volScalarField& /*T*/
)
{
  mu_ == mu0_;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "newtonian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
namespace viscosityModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(Newtonian, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(viscosityModel, Newtonian, dictionary);
}
}
// Constructors 
mousse::viscosityModels::Newtonian::Newtonian
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  viscosityModel(name, viscosityProperties, U, phi),
  nu0_("nu", dimViscosity, viscosityProperties_),
  nu_
  (
    IOobject
    (
      name,
      U_.time().timeName(),
      U_.db(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    U_.mesh(),
    nu0_
  )
{}
// Member Functions 
bool mousse::viscosityModels::Newtonian::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  viscosityProperties_.lookup("nu") >> nu0_;
  nu_ = nu0_;
  return true;
}

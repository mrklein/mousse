// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#if defined(__GNUC__)
#include "surface_fields.hpp"
#endif
#include "slurry.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace mixtureViscosityModels
{
DEFINE_TYPE_NAME_AND_DEBUG(slurry, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  mixtureViscosityModel,
  slurry,
  dictionary
);
}
}
// Constructors 
mousse::mixtureViscosityModels::slurry::slurry
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi,
  const word /*modelName*/
)
:
  mixtureViscosityModel{name, viscosityProperties, U, phi},
  alpha_
  {
    U.mesh().lookupObject<volScalarField>
    (
      IOobject::groupName
      (
        viscosityProperties.lookupOrDefault<word>("alpha", "alpha"),
        viscosityProperties.dictName()
      )
    )
  }
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::mixtureViscosityModels::slurry::mu(const volScalarField& muc) const
{
  return
    muc*(1.0 + 2.5*alpha_ + 10.05*sqr(alpha_) + 0.00273*exp(16.6*alpha_));
}
bool mousse::mixtureViscosityModels::slurry::read
(
  const dictionary& /*viscosityProperties*/
)
{
  return true;
}

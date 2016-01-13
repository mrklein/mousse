// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dripping_injection.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "mathematical_constants.hpp"
#include "random.hpp"
#include "vol_fields.hpp"
#include "kinematic_single_layer.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(drippingInjection, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(injectionModel, drippingInjection, dictionary);
// Constructors 
drippingInjection::drippingInjection
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  injectionModel(type(), owner, dict),
  deltaStable_(readScalar(coeffDict_.lookup("deltaStable"))),
  particlesPerParcel_(readScalar(coeffDict_.lookup("particlesPerParcel"))),
  rndGen_(label(0), -1),
  parcelDistribution_
  (
    distributionModels::distributionModel::New
    (
      coeffDict_.subDict("parcelDistribution"),
      rndGen_
    )
  ),
  diameter_(owner.regionMesh().nCells(), -1.0)
{}
// Destructor 
drippingInjection::~drippingInjection()
{}
// Member Functions 
void drippingInjection::correct
(
  scalarField& availableMass,
  scalarField& massToInject,
  scalarField& diameterToInject
)
{
  const kinematicSingleLayer& film =
    refCast<const kinematicSingleLayer>(this->owner());
  const scalar pi = constant::mathematical::pi;
  // calculate available dripping mass
  tmp<volScalarField> tgNorm(film.gNorm());
  const scalarField& gNorm = tgNorm();
  const scalarField& magSf = film.magSf();
  const scalarField& delta = film.delta();
  const scalarField& rho = film.rho();
  scalarField massDrip(film.regionMesh().nCells(), 0.0);
  FOR_ALL(gNorm, i)
  {
    if (gNorm[i] > SMALL)
    {
      const scalar ddelta = max(0.0, delta[i] - deltaStable_);
      massDrip[i] +=
        min(availableMass[i], max(0.0, ddelta*rho[i]*magSf[i]));
    }
  }
  // Collect the data to be transferred
  FOR_ALL(massDrip, cellI)
  {
    if (massDrip[cellI] > 0)
    {
      // set new particle diameter if not already set
      if (diameter_[cellI] < 0)
      {
        diameter_[cellI] = parcelDistribution_->sample();
      }
      scalar& diam = diameter_[cellI];
      scalar rhoc = rho[cellI];
      scalar minMass = particlesPerParcel_*rhoc*pi/6*pow3(diam);
      if (massDrip[cellI] > minMass)
      {
        // All drip mass can be injected
        massToInject[cellI] += massDrip[cellI];
        availableMass[cellI] -= massDrip[cellI];
        // Set particle diameter
        diameterToInject[cellI] = diam;
        // Retrieve new particle diameter sample
        diam = parcelDistribution_->sample();
        addToInjectedMass(massDrip[cellI]);
      }
      else
      {
        // Particle mass below minimum threshold - cannot be injected
        massToInject[cellI] = 0.0;
        diameterToInject[cellI] = 0.0;
      }
    }
    else
    {
      massToInject[cellI] = 0.0;
      diameterToInject[cellI] = 0.0;
    }
  }
  injectionModel::correct();
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solidification.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "thermo_single_layer.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(solidification, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  phaseChangeModel,
  solidification,
  dictionary
);


// Constructors 
solidification::solidification
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  phaseChangeModel{typeName, owner, dict},
  T0_{readScalar(coeffDict_.lookup("T0"))},
  maxSolidificationFrac_
  {
    coeffDict_.lookupOrDefault("maxSolidificationFrac", 0.2)
  },
  maxSolidificationRate_
  {
    dimensioned<scalar>::lookupOrDefault
    (
      "maxSolidificationRate",
      coeffDict_,
      dimless/dimTime,
      GREAT
    )
  },
  mass_
  {
    {
      typeName + ":mass",
      owner.regionMesh().time().timeName(),
      owner.regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    owner.regionMesh(),
    {"zero", dimMass, 0.0},
    zeroGradientFvPatchScalarField::typeName
  },
  thickness_
  {
    {
      typeName + ":thickness",
      owner.regionMesh().time().timeName(),
      owner.regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    owner.regionMesh(),
    {"zero", dimLength, 0.0},
    zeroGradientFvPatchScalarField::typeName
  }
{}


// Destructor 
solidification::~solidification()
{}


// Member Functions 
void solidification::correctModel
(
  const scalar /*dt*/,
  scalarField& availableMass,
  scalarField& dMass,
  scalarField& /*dEnergy*/
)
{
  const thermoSingleLayer& film = filmType<thermoSingleLayer>();
  const scalarField& T = film.T();
  const scalarField& alpha = film.alpha();
  const scalar rateLimiter =
    min
    (
      maxSolidificationFrac_,
      (
        maxSolidificationRate_*owner_.regionMesh().time().deltaTValue()
      ).value()
    );
  FOR_ALL(alpha, celli) {
    if (alpha[celli] > 0.5) {
      if (T[celli] < T0_) {
        const scalar dm = rateLimiter*availableMass[celli];
        mass_[celli] += dm;
        dMass[celli] += dm;
        // Heat is assumed to be removed by heat-transfer to the wall
        // so the energy remains unchanged by the phase-change.
      }
    }
  }
  thickness_ = mass_/film.magSf()/film.rho();
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse


// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_two_phase_interacting_mixture.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "fvc.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(incompressibleTwoPhaseInteractingMixture, 0);

}


// Constructors 
mousse::incompressibleTwoPhaseInteractingMixture::
incompressibleTwoPhaseInteractingMixture
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  IOdictionary
  {
    {
      "transportProperties",
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  },
  twoPhaseMixture{U.mesh(), *this},
  muModel_
  {
    mixtureViscosityModel::New
    (
      "mu",
      subDict(phase1Name_),
      U,
      phi
    )
  },
  nucModel_
  {
    viscosityModel::New
    (
      "nuc",
      subDict(phase2Name_),
      U,
      phi
    )
  },
  rhod_{"rho", dimDensity, muModel_->viscosityProperties()},
  rhoc_{"rho", dimDensity, nucModel_->viscosityProperties()},
  dd_
  {
    "d",
    dimLength,
    muModel_->viscosityProperties().lookupOrDefault("d", 0.0)
  },
  alphaMax_{muModel_->viscosityProperties().lookupOrDefault("alphaMax", 1.0)},
  U_{U},
  phi_{phi},
  mu_
  {
    {
      "mu",
      U_.time().timeName(),
      U_.db()
    },
    U_.mesh(),
    {"mu", dimensionSet{1, -1, -1, 0, 0}, 0},
    calculatedFvPatchScalarField::typeName
  }
{
  correct();
}


// Member Functions 
bool mousse::incompressibleTwoPhaseInteractingMixture::read()
{
  if (regIOobject::read()) {
    if (muModel_().read(subDict(phase1Name_))
        && nucModel_().read(subDict(phase2Name_))) {
      muModel_->viscosityProperties().lookup("rho") >> rhod_;
      nucModel_->viscosityProperties().lookup("rho") >> rhoc_;
      dd_ = dimensionedScalar
      {
        "d",
        dimLength,
        muModel_->viscosityProperties().lookupOrDefault<double>("d", 0)
      };
      alphaMax_ =
        muModel_->viscosityProperties().lookupOrDefault
        (
          "alphaMax",
          1.0
        );
      return true;
    }
    return false;
  }
  return false;
}


// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "eddy_diffusivity.hpp"


// Protected Member Functions 
template<class BasicTurbulenceModel>
void mousse::EddyDiffusivity<BasicTurbulenceModel>::correctNut()
{
  // Read Prt if provided
  Prt_ =
    dimensioned<scalar>::lookupOrDefault
    (
      "Prt",
      this->coeffDict(),
      1.0
    );
  alphat_ = this->rho_*this->nut()/Prt_;
  alphat_.correctBoundaryConditions();
}


// Constructors 
template<class BasicTurbulenceModel>
mousse::EddyDiffusivity<BasicTurbulenceModel>::EddyDiffusivity
(
  const word& type,
  const alphaField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  BasicTurbulenceModel
  {
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  },
  // Cannot read Prt yet
  Prt_{"Prt", dimless, 1.0},
  alphat_
  {
    IOobject
    {
      IOobject::groupName("alphat", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  }
{}


// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::EddyDiffusivity<BasicTurbulenceModel>::read()
{
  if (BasicTurbulenceModel::read()) {
    Prt_.readIfPresent(this->coeffDict());
    return true;
  } else {
    return false;
  }
}


template<class BasicTurbulenceModel>
void mousse::EddyDiffusivity<BasicTurbulenceModel>::correctEnergyTransport()
{
  EddyDiffusivity<BasicTurbulenceModel>::correctNut();
}


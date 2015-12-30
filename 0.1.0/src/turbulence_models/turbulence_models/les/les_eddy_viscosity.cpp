// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "les_eddy_viscosity.hpp"
namespace mousse
{
namespace LESModels
{
// Constructors 
template<class BasicTurbulenceModel>
LESeddyViscosity<BasicTurbulenceModel>::LESeddyViscosity
(
  const word& type,
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  eddyViscosity<LESModel<BasicTurbulenceModel> >
  (
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  ),
  Ce_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Ce",
      this->coeffDict_,
      1.048
    )
  )
{}
// Member Functions 
template<class BasicTurbulenceModel>
bool LESeddyViscosity<BasicTurbulenceModel>::read()
{
  if (eddyViscosity<LESModel<BasicTurbulenceModel> >::read())
  {
    Ce_.readIfPresent(this->coeffDict());
    return true;
  }
  else
  {
    return false;
  }
}
template<class BasicTurbulenceModel>
tmp<volScalarField> LESeddyViscosity<BasicTurbulenceModel>::epsilon() const
{
  tmp<volScalarField> tk(this->k());
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("epsilon", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      Ce_*tk()*sqrt(tk())/this->delta()
    )
  );
}
}  // namespace LESModels
}  // namespace mousse

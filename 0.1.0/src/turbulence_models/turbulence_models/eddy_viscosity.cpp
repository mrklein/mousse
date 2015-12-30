// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "eddy_viscosity.hpp"
#include "fvc.hpp"
#include "fvm.hpp"
// Constructors 
template<class BasicTurbulenceModel>
mousse::eddyViscosity<BasicTurbulenceModel>::eddyViscosity
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
  linearViscousStress<BasicTurbulenceModel>
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
  nut_
  (
    IOobject
    (
      IOobject::groupName("nut", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    this->mesh_
  )
{}
// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::eddyViscosity<BasicTurbulenceModel>::read()
{
  return BasicTurbulenceModel::read();
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::eddyViscosity<BasicTurbulenceModel>::R() const
{
  tmp<volScalarField> tk(k());
  // Get list of patchField type names from k
  wordList patchFieldTypes(tk().boundaryField().types());
  // For k patchField types which do not have an equivalent for symmTensor
  // set to calculated
  forAll(patchFieldTypes, i)
  {
    if
    (
     !fvPatchField<symmTensor>::patchConstructorTablePtr_
        ->found(patchFieldTypes[i])
    )
    {
      patchFieldTypes[i] = calculatedFvPatchField<symmTensor>::typeName;
    }
  }
  return tmp<volSymmTensorField>
  (
    new volSymmTensorField
    (
      IOobject
      (
        IOobject::groupName("R", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      ((2.0/3.0)*I)*tk() - (nut_)*dev(twoSymm(fvc::grad(this->U_))),
      patchFieldTypes
    )
  );
}
template<class BasicTurbulenceModel>
void mousse::eddyViscosity<BasicTurbulenceModel>::correct()
{
  BasicTurbulenceModel::correct();
}

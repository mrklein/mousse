// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pasr.hpp"
#include "fvm_sup.hpp"
// Constructors 
template<class Type>
mousse::combustionModels::PaSR<Type>::PaSR
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  laminar<Type>(modelType, mesh, phaseName),
  Cmix_(readScalar(this->coeffs().lookup("Cmix"))),
  turbulentReaction_(this->coeffs().lookup("turbulentReaction")),
  kappa_
  (
    IOobject
    (
      IOobject::groupName("PaSR:kappa", phaseName),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("kappa", dimless, 0.0)
  )
{}
// Destructor 
template<class Type>
mousse::combustionModels::PaSR<Type>::~PaSR()
{}
// Member Functions 
template<class Type>
void mousse::combustionModels::PaSR<Type>::correct()
{
  if (this->active())
  {
    laminar<Type>::correct();
    if (turbulentReaction_)
    {
      tmp<volScalarField> tepsilon(this->turbulence().epsilon());
      const volScalarField& epsilon = tepsilon();
      tmp<volScalarField> tmuEff(this->turbulence().muEff());
      const volScalarField& muEff = tmuEff();
      tmp<volScalarField> ttc(this->tc());
      const volScalarField& tc = ttc();
      tmp<volScalarField> trho(this->rho());
      const volScalarField& rho = trho();
      forAll(epsilon, i)
      {
        scalar tk =
          Cmix_*sqrt(max(muEff[i]/rho[i]/(epsilon[i] + SMALL), 0));
        if (tk > SMALL)
        {
          kappa_[i] = tc[i]/(tc[i] + tk);
        }
        else
        {
          kappa_[i] = 1.0;
        }
      }
    }
    else
    {
      kappa_ = 1.0;
    }
  }
}
template<class Type>
mousse::tmp<mousse::fvScalarMatrix>
mousse::combustionModels::PaSR<Type>::R(volScalarField& Y) const
{
  return kappa_*laminar<Type>::R(Y);
}
template<class Type>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::PaSR<Type>::dQ() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject::groupName("PaSR:dQ", this->phaseName_),
      kappa_*laminar<Type>::dQ()
    )
  );
}
template<class Type>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::PaSR<Type>::Sh() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject::groupName("PaSR:Sh", this->phaseName_),
      kappa_*laminar<Type>::Sh()
    )
  );
}
template<class Type>
bool mousse::combustionModels::PaSR<Type>::read()
{
  if (laminar<Type>::read())
  {
    this->coeffs().lookup("Cmix") >> Cmix_;
    this->coeffs().lookup("turbulentReaction") >> turbulentReaction_;
    return true;
  }
  else
  {
    return false;
  }
}

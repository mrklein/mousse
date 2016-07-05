// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laminar.hpp"
#include "fvm_sup.hpp"
#include "local_euler_ddt_scheme.hpp"


// Constructors 
template<class Type>
mousse::combustionModels::laminar<Type>::laminar
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  Type{modelType, mesh, phaseName},
  integrateReactionRate_
  {
    this->coeffs().lookupOrDefault("integrateReactionRate", true)
  }
{
  if (integrateReactionRate_) {
    Info << "    using integrated reaction rate" << endl;
  } else {
    Info << "    using instantaneous reaction rate" << endl;
  }
}


// Destructor 
template<class Type>
mousse::combustionModels::laminar<Type>::~laminar()
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::laminar<Type>::tc() const
{
  return this->chemistryPtr_->tc();
}


template<class Type>
void mousse::combustionModels::laminar<Type>::correct()
{
  if (!this->active())
    return;
  if (integrateReactionRate_) {
    if (fv::localEulerDdt::enabled(this->mesh())) {
      const scalarField& rDeltaT =
        fv::localEulerDdt::localRDeltaT(this->mesh());
      if (this->coeffs().found("maxIntegrationTime")) {
        scalar maxIntegrationTime
        {
          readScalar(this->coeffs().lookup("maxIntegrationTime"))
        };
        this->chemistryPtr_->solve(min(1.0/rDeltaT, maxIntegrationTime)());
      } else {
        this->chemistryPtr_->solve((1.0/rDeltaT)());
      }
    } else {
      this->chemistryPtr_->solve(this->mesh().time().deltaTValue());
    }
  } else {
    this->chemistryPtr_->calculate();
  }
}


template<class Type>
mousse::tmp<mousse::fvScalarMatrix>
mousse::combustionModels::laminar<Type>::R(volScalarField& Y) const
{
  tmp<fvScalarMatrix> tSu{new fvScalarMatrix{Y, dimMass/dimTime}};
  fvScalarMatrix& Su = tSu();
  if (this->active()) {
    const label specieI = this->thermo().composition().species()[Y.member()];
    Su += this->chemistryPtr_->RR(specieI);
  }
  return tSu;
}


template<class Type>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::laminar<Type>::dQ() const
{
  tmp<volScalarField> tdQ
  {
    new volScalarField
    {
      {
        IOobject::groupName(typeName + ":dQ", this->phaseName_),
        this->mesh().time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"dQ", dimEnergy/dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  if (this->active()) {
    tdQ() = this->chemistryPtr_->dQ();
  }
  return tdQ;
}


template<class Type>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::laminar<Type>::Sh() const
{
  tmp<volScalarField> tSh
  {
    new volScalarField
    {
      {
        IOobject::groupName(typeName + ":Sh", this->phaseName_),
        this->mesh().time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimEnergy/dimTime/dimVolume, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  if (this->active()) {
    tSh() = this->chemistryPtr_->Sh();
  }
  return tSh;
}


template<class Type>
bool mousse::combustionModels::laminar<Type>::read()
{
  if (Type::read()) {
    this->coeffs().lookup("integrateReactionRate") >> integrateReactionRate_;
    return true;
  }
  return false;
}


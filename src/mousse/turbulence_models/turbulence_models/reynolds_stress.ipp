// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reynolds_stress.hpp"
#include "fvc.hpp"
#include "fvm.hpp"
#include "wall_fv_patch.hpp"


// Protected Member Functions 
template<class BasicTurbulenceModel>
void mousse::ReynoldsStress<BasicTurbulenceModel>::boundNormalStress
(
  volSymmTensorField& R
) const
{
  scalar kMin = this->kMin_.value();
  R.max
  (
    dimensionedSymmTensor
    {
      "zero",
      R.dimensions(),
      symmTensor
      {
        kMin, -GREAT, -GREAT,
        kMin, -GREAT,
        kMin
      }
    }
  );
}


template<class BasicTurbulenceModel>
void mousse::ReynoldsStress<BasicTurbulenceModel>::correctWallShearStress
(
  volSymmTensorField& R
) const
{
  const fvPatchList& patches = this->mesh_.boundary();
  FOR_ALL(patches, patchi) {
    const fvPatch& curPatch = patches[patchi];
    if (isA<wallFvPatch>(curPatch)) {
      symmTensorField& Rw = R.boundaryField()[patchi];
      const scalarField& nutw = this->nut_.boundaryField()[patchi];
      const vectorField snGradU{this->U_.boundaryField()[patchi].snGrad()};
      const vectorField& faceAreas
        = this->mesh_.Sf().boundaryField()[patchi];
      const scalarField& magFaceAreas
        = this->mesh_.magSf().boundaryField()[patchi];
      FOR_ALL(curPatch, facei) {
        // Calculate near-wall velocity gradient
        tensor gradUw = (faceAreas[facei]/magFaceAreas[facei])*snGradU[facei];
        // Calculate near-wall shear-stress tensor
        tensor tauw = -nutw[facei]*2*dev(symm(gradUw));
        // Reset the shear components of the stress tensor
        Rw[facei].xy() = tauw.xy();
        Rw[facei].xz() = tauw.xz();
        Rw[facei].yz() = tauw.yz();
      }
    }
  }
}


// Constructors 
template<class BasicTurbulenceModel>
mousse::ReynoldsStress<BasicTurbulenceModel>::ReynoldsStress
(
  const word& modelName,
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  BasicTurbulenceModel
  {
    modelName,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  },
  couplingFactor_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "couplingFactor",
      this->coeffDict_,
      0.0
    )
  },
  R_
  {
    {
      IOobject::groupName("R", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  },
  nut_
  {
    {
      IOobject::groupName("nut", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  }
{
  if (couplingFactor_.value() < 0.0 || couplingFactor_.value() > 1.0) {
    FATAL_ERROR_IN
    (
      "ReynoldsStress::ReynoldsStress"
    )
    << "couplingFactor = " << couplingFactor_
    << " is not in range 0 - 1" << nl
    << exit(FatalError);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::ReynoldsStress<BasicTurbulenceModel>::read()
{
  return BasicTurbulenceModel::read();
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::ReynoldsStress<BasicTurbulenceModel>::R() const
{
  return R_;
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::ReynoldsStress<BasicTurbulenceModel>::k() const
{
  tmp<mousse::volScalarField> tk{0.5*tr(R_)};
  tk().rename("k");
  return tk;
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::ReynoldsStress<BasicTurbulenceModel>::devRhoReff() const
{
  return
    tmp<volSymmTensorField>
    {
      new volSymmTensorField
      {
        {
          IOobject::groupName("devRhoReff", this->U_.group()),
          this->runTime_.timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        this->alpha_*this->rho_*R_
        - (this->alpha_*this->rho_*this->nu())
        *dev(twoSymm(fvc::grad(this->U_)))
      }
    };
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::ReynoldsStress<BasicTurbulenceModel>::divDevRhoReff
(
  volVectorField& U
) const
{
  if (couplingFactor_.value() > 0.0) {
    return
    (
      fvc::div
      (
        this->alpha_*this->rho_*R_
       + couplingFactor_
       *this->alpha_*this->rho_*this->nut()*fvc::grad(U),
        "div(devRhoReff)"
      )
      + fvc::laplacian
      (
        (1.0 - couplingFactor_)*this->alpha_*this->rho_*this->nut(),
        U,
        "laplacian(nuEff,U)"
      )
      - fvm::laplacian(this->alpha_*this->rho_*this->nuEff(), U)
      - fvc::div(this->alpha_*this->rho_*this->nu()*dev2(T(fvc::grad(U))))
    );
  } else {
    return
    (
      fvc::div(this->alpha_*this->rho_*R_)
      + fvc::laplacian
      (
        this->alpha_*this->rho_*this->nut(),
        U,
        "laplacian(nuEff,U)"
      )
      - fvm::laplacian(this->alpha_*this->rho_*this->nuEff(), U)
      - fvc::div(this->alpha_*this->rho_*this->nu()*dev2(T(fvc::grad(U))))
    );
  }
  return -fvm::laplacian(this->alpha_*this->rho_*this->nuEff(), U)
    - fvc::div((this->alpha_*this->rho_*this->nuEff())*dev2(T(fvc::grad(U))));
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::ReynoldsStress<BasicTurbulenceModel>::divDevRhoReff
(
  const volScalarField& rho,
  volVectorField& U
) const
{
  return -fvm::laplacian(this->alpha_*rho*this->nuEff(), U)
    - fvc::div((this->alpha_*rho*this->nuEff())*dev2(T(fvc::grad(U))));
}


template<class BasicTurbulenceModel>
void mousse::ReynoldsStress<BasicTurbulenceModel>::correct()
{
  BasicTurbulenceModel::correct();
}


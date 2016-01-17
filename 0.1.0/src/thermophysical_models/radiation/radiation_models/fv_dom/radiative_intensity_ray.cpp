// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radiative_intensity_ray.hpp"
#include "fvm.hpp"
#include "fv_dom.hpp"
#include "constants.hpp"
using namespace mousse::constant;
const mousse::word mousse::radiation::radiativeIntensityRay::intensityPrefix
(
  "ILambda"
);
// Constructors 
mousse::radiation::radiativeIntensityRay::radiativeIntensityRay
(
  const fvDOM& dom,
  const fvMesh& mesh,
  const scalar phi,
  const scalar theta,
  const scalar deltaPhi,
  const scalar deltaTheta,
  const label nLambda,
  const absorptionEmissionModel& absorptionEmission,
  const blackBodyEmission& blackBody,
  const label rayId
)
:
  dom_(dom),
  mesh_(mesh),
  absorptionEmission_(absorptionEmission),
  blackBody_(blackBody),
  I_
  (
    IOobject
    (
      "I" + name(rayId),
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensionedScalar("I", dimMass/pow3(dimTime), 0.0)
  ),
  Qr_
  (
    IOobject
    (
      "Qr" + name(rayId),
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensionedScalar("Qr", dimMass/pow3(dimTime), 0.0)
  ),
  Qin_
  (
    IOobject
    (
      "Qin" + name(rayId),
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensionedScalar("Qin", dimMass/pow3(dimTime), 0.0)
  ),
  Qem_
  (
    IOobject
    (
      "Qem" + name(rayId),
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh_,
    dimensionedScalar("Qem", dimMass/pow3(dimTime), 0.0)
  ),
  d_(vector::zero),
  dAve_(vector::zero),
  theta_(theta),
  phi_(phi),
  omega_(0.0),
  nLambda_(nLambda),
  ILambda_(nLambda),
  myRayId_(rayId)
{
  scalar sinTheta = mousse::sin(theta);
  scalar cosTheta = mousse::cos(theta);
  scalar sinPhi = mousse::sin(phi);
  scalar cosPhi = mousse::cos(phi);
  omega_ = 2.0*sinTheta*mousse::sin(deltaTheta/2.0)*deltaPhi;
  d_ = vector(sinTheta*sinPhi, sinTheta*cosPhi, cosTheta);
  dAve_ = vector
  (
    sinPhi
   *mousse::sin(0.5*deltaPhi)
   *(deltaTheta - mousse::cos(2.0*theta)
   *mousse::sin(deltaTheta)),
    cosPhi
   *mousse::sin(0.5*deltaPhi)
   *(deltaTheta - mousse::cos(2.0*theta)
   *mousse::sin(deltaTheta)),
    0.5*deltaPhi*mousse::sin(2.0*theta)*mousse::sin(deltaTheta)
  );
  autoPtr<volScalarField> IDefaultPtr;
  FOR_ALL(ILambda_, lambdaI)
  {
    IOobject IHeader
    (
      intensityPrefix + "_" + name(rayId) + "_" + name(lambdaI),
      mesh_.time().timeName(),
      mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    );
    // Check if field exists and can be read
    if (IHeader.headerOk())
    {
      ILambda_.set
      (
        lambdaI,
        new volScalarField(IHeader, mesh_)
      );
    }
    else
    {
      // Demand driven load the IDefault field
      if (!IDefaultPtr.valid())
      {
        IDefaultPtr.reset
        (
          new volScalarField
          (
            IOobject
            (
              "IDefault",
              mesh_.time().timeName(),
              mesh_,
              IOobject::MUST_READ,
              IOobject::NO_WRITE
            ),
            mesh_
          )
        );
      }
      // Reset the MUST_READ flag
      IOobject noReadHeader(IHeader);
      noReadHeader.readOpt() = IOobject::NO_READ;
      ILambda_.set
      (
        lambdaI,
        new volScalarField(noReadHeader, IDefaultPtr())
      );
    }
  }
}
// Destructor 
mousse::radiation::radiativeIntensityRay::~radiativeIntensityRay()
{}
// Member Functions 
mousse::scalar mousse::radiation::radiativeIntensityRay::correct()
{
  // Reset boundary heat flux to zero
  Qr_.boundaryField() = 0.0;
  scalar maxResidual = -GREAT;
  FOR_ALL(ILambda_, lambdaI)
  {
    const volScalarField& k = dom_.aLambda(lambdaI);
    tmp<fvScalarMatrix> IiEq;
    if (!dom_.cacheDiv())
    {
      const surfaceScalarField Ji(dAve_ & mesh_.Sf());
      IiEq =
      (
        fvm::div(Ji, ILambda_[lambdaI], "div(Ji,Ii_h)")
       + fvm::Sp(k*omega_, ILambda_[lambdaI])
      ==
        1.0/constant::mathematical::pi*omega_
       *(
          // Remove aDisp from k
          (k - absorptionEmission_.aDisp(lambdaI))
         *blackBody_.bLambda(lambdaI)
         + absorptionEmission_.E(lambdaI)/4
        )
      );
    }
    else
    {
      IiEq =
      (
       dom_.fvRayDiv(myRayId_, lambdaI)
      + fvm::Sp(k*omega_, ILambda_[lambdaI])
     ==
       1.0/constant::mathematical::pi*omega_
      * (
         // Remove aDisp from k
         (k - absorptionEmission_.aDisp(lambdaI))
         *blackBody_.bLambda(lambdaI)
        + absorptionEmission_.E(lambdaI)/4
       )
      );
    }
    IiEq().relax();
    const solverPerformance ILambdaSol = solve
    (
      IiEq(),
      mesh_.solver("Ii")
    );
    const scalar initialRes =
      ILambdaSol.initialResidual()*omega_/dom_.omegaMax();
    maxResidual = max(initialRes, maxResidual);
  }
  return maxResidual;
}
void mousse::radiation::radiativeIntensityRay::addIntensity()
{
  I_ = dimensionedScalar("zero", dimMass/pow3(dimTime), 0.0);
  FOR_ALL(ILambda_, lambdaI)
  {
    I_ += ILambda_[lambdaI];
  }
}

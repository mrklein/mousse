// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_pressure_gradient_force.hpp"
#include "fvc_ddt.hpp"
#include "fvc_grad.hpp"
// Constructors 
template<class CloudType>
mousse::PressureGradientForce<CloudType>::PressureGradientForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType
)
:
  ParticleForce<CloudType>(owner, mesh, dict, forceType, true),
  UName_(this->coeffs().template lookupOrDefault<word>("U", "U")),
  DUcDtInterpPtr_(NULL)
{}
template<class CloudType>
mousse::PressureGradientForce<CloudType>::PressureGradientForce
(
  const PressureGradientForce& pgf
)
:
  ParticleForce<CloudType>(pgf),
  UName_(pgf.UName_),
  DUcDtInterpPtr_(NULL)
{}
// Destructor 
template<class CloudType>
mousse::PressureGradientForce<CloudType>::~PressureGradientForce()
{}
// Member Functions 
template<class CloudType>
void mousse::PressureGradientForce<CloudType>::cacheFields(const bool store)
{
  static word fName("DUcDt");
  bool fieldExists = this->mesh().template foundObject<volVectorField>(fName);
  if (store)
  {
    if (!fieldExists)
    {
      const volVectorField& Uc = this->mesh().template
        lookupObject<volVectorField>(UName_);
      volVectorField* DUcDtPtr = new volVectorField
      (
        fName,
        fvc::ddt(Uc) + (Uc & fvc::grad(Uc))
      );
      DUcDtPtr->store();
    }
    const volVectorField& DUcDt = this->mesh().template
      lookupObject<volVectorField>(fName);
    DUcDtInterpPtr_.reset
    (
      interpolation<vector>::New
      (
        this->owner().solution().interpolationSchemes(),
        DUcDt
      ).ptr()
    );
  }
  else
  {
    DUcDtInterpPtr_.clear();
    if (fieldExists)
    {
      const volVectorField& DUcDt = this->mesh().template
        lookupObject<volVectorField>(fName);
      const_cast<volVectorField&>(DUcDt).checkOut();
    }
  }
}
template<class CloudType>
mousse::forceSuSp mousse::PressureGradientForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  vector DUcDt =
    DUcDtInterp().interpolate(p.position(), p.currentTetIndices());
  value.Su() = mass*p.rhoc()/p.rho()*DUcDt;
  return value;
}
template<class CloudType>
mousse::scalar mousse::PressureGradientForce<CloudType>::massAdd
(
  const typename CloudType::parcelType&,
  const scalar
) const
{
  return 0.0;
}

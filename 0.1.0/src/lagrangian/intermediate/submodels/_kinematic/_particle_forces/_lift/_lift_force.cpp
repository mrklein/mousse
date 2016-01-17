// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_lift_force.hpp"
#include "fvc_curl.hpp"
// Protected Member Functions 
template<class CloudType>
mousse::scalar mousse::LiftForce<CloudType>::LiftForce::Cl
(
  const typename CloudType::parcelType& /*p*/,
  const vector& /*curlUc*/,
  const scalar /*Re*/,
  const scalar /*muc*/
) const
{
  // dummy
  return 0.0;
}
// Constructors 
template<class CloudType>
mousse::LiftForce<CloudType>::LiftForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType
)
:
  ParticleForce<CloudType>(owner, mesh, dict, forceType, true),
  UName_(this->coeffs().template lookupOrDefault<word>("U", "U")),
  curlUcInterpPtr_(NULL)
{}
template<class CloudType>
mousse::LiftForce<CloudType>::LiftForce(const LiftForce& lf)
:
  ParticleForce<CloudType>(lf),
  UName_(lf.UName_),
  curlUcInterpPtr_(NULL)
{}
// Destructor 
template<class CloudType>
mousse::LiftForce<CloudType>::~LiftForce()
{}
// Member Functions 
template<class CloudType>
void mousse::LiftForce<CloudType>::cacheFields(const bool store)
{
  static word fName("curlUcDt");
  bool fieldExists = this->mesh().template foundObject<volVectorField>(fName);
  if (store)
  {
    if (!fieldExists)
    {
      const volVectorField& Uc = this->mesh().template
        lookupObject<volVectorField>(UName_);
      volVectorField* curlUcPtr =
        new volVectorField(fName, fvc::curl(Uc));
      curlUcPtr->store();
    }
    const volVectorField& curlUc = this->mesh().template
      lookupObject<volVectorField>(fName);
    curlUcInterpPtr_.reset
    (
      interpolation<vector>::New
      (
        this->owner().solution().interpolationSchemes(),
        curlUc
      ).ptr()
    );
  }
  else
  {
    curlUcInterpPtr_.clear();
    if (fieldExists)
    {
      const volVectorField& curlUc = this->mesh().template
        lookupObject<volVectorField>(fName);
      const_cast<volVectorField&>(curlUc).checkOut();
    }
  }
}
template<class CloudType>
mousse::forceSuSp mousse::LiftForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  vector curlUc =
    curlUcInterp().interpolate(p.position(), p.currentTetIndices());
  scalar Cl = this->Cl(p, curlUc, Re, muc);
  value.Su() = mass/p.rho()*p.rhoc()*Cl*((p.Uc() - p.U())^curlUc);
  return value;
}

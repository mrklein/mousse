// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_trap.hpp"
#include "fvc_grad.hpp"
// Constructors 
template<class CloudType>
mousse::ParticleTrap<CloudType>::ParticleTrap
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
  alphaName_
  (
    this->coeffDict().template lookupOrDefault<word>("alphaName", "alpha")
  ),
  alphaPtr_(NULL),
  gradAlphaPtr_(NULL),
  threshold_(readScalar(this->coeffDict().lookup("threshold")))
{}
template<class CloudType>
mousse::ParticleTrap<CloudType>::ParticleTrap
(
  const ParticleTrap<CloudType>& pt
)
:
  CloudFunctionObject<CloudType>(pt),
  alphaName_(pt.alphaName_),
  alphaPtr_(pt.alphaPtr_),
  gradAlphaPtr_(NULL),
  threshold_(pt.threshold_)
{}
// Destructor 
template<class CloudType>
mousse::ParticleTrap<CloudType>::~ParticleTrap()
{}
// Member Functions 
template<class CloudType>
void mousse::ParticleTrap<CloudType>::preEvolve()
{
  if (alphaPtr_ == NULL)
  {
    const fvMesh& mesh = this->owner().mesh();
    const volScalarField& alpha =
      mesh.lookupObject<volScalarField>(alphaName_);
    alphaPtr_ = &alpha;
  }
  if (gradAlphaPtr_.valid())
  {
    gradAlphaPtr_() == fvc::grad(*alphaPtr_);
  }
  else
  {
    gradAlphaPtr_.reset(new volVectorField(fvc::grad(*alphaPtr_)));
  }
}
template<class CloudType>
void mousse::ParticleTrap<CloudType>::postEvolve()
{
  gradAlphaPtr_.clear();
}
template<class CloudType>
void mousse::ParticleTrap<CloudType>::postMove
(
  parcelType& p,
  const label cellI,
  const scalar,
  const point&,
  bool&
)
{
  if (alphaPtr_->internalField()[cellI] < threshold_)
  {
    const vector& gradAlpha = gradAlphaPtr_()[cellI];
    vector nHat = gradAlpha/mag(gradAlpha);
    scalar nHatU = nHat & p.U();
    if (nHatU < 0)
    {
      p.U() -= 2*nHat*nHatU;
    }
  }
}

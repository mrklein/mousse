// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_void_fraction.hpp"


// Protectd Member Functions
template<class CloudType>
void mousse::VoidFraction<CloudType>::write()
{
  if (thetaPtr_.valid()) {
    thetaPtr_->write();
  } else {
    FATAL_ERROR_IN("void mousse::VoidFraction<CloudType>::write()")
      << "thetaPtr not valid" << abort(FatalError);
  }
}


// Constructors 
template<class CloudType>
mousse::VoidFraction<CloudType>::VoidFraction
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>{dict, owner, modelName, typeName},
  thetaPtr_{nullptr}
{}


template<class CloudType>
mousse::VoidFraction<CloudType>::VoidFraction
(
  const VoidFraction<CloudType>& vf
)
:
  CloudFunctionObject<CloudType>{vf},
  thetaPtr_{nullptr}
{}


// Destructor 
template<class CloudType>
mousse::VoidFraction<CloudType>::~VoidFraction()
{}


// Member Functions 
template<class CloudType>
void mousse::VoidFraction<CloudType>::preEvolve()
{
  if (thetaPtr_.valid()) {
    thetaPtr_->internalField() = 0.0;
  } else {
    const fvMesh& mesh = this->owner().mesh();
    thetaPtr_.reset
    (
      new volScalarField
      {
        {
          this->owner().name() + "Theta",
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"zero", dimless, 0.0}
      }
    );
  }
}


template<class CloudType>
void mousse::VoidFraction<CloudType>::postEvolve()
{
  volScalarField& theta = thetaPtr_();
  const fvMesh& mesh = this->owner().mesh();
  theta.internalField() /= mesh.time().deltaTValue()*mesh.V();
  CloudFunctionObject<CloudType>::postEvolve();
}


template<class CloudType>
void mousse::VoidFraction<CloudType>::postMove
(
  parcelType& p,
  const label cellI,
  const scalar dt,
  const point&,
  bool&
)
{
  volScalarField& theta = thetaPtr_();
  theta[cellI] += dt*p.nParticle()*p.volume();
}


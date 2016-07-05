// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_non_inertial_frame_force.hpp"
#include "uniform_dimensioned_fields.hpp"


// Constructors 
template<class CloudType>
mousse::NonInertialFrameForce<CloudType>::NonInertialFrameForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>{owner, mesh, dict, typeName, true},
  WName_
  {
    this->coeffs().template lookupOrDefault<word>
    (
      "linearAccelerationName",
      "linearAcceleration"
    )
  },
  W_{vector::zero},
  omegaName_
  {
    this->coeffs().template lookupOrDefault<word>
    (
      "angularVelocityName",
      "angularVelocity"
    )
  },
  omega_{vector::zero},
  omegaDotName_
  {
    this->coeffs().template lookupOrDefault<word>
    (
      "angularAccelerationName",
      "angularAcceleration"
    )
  },
  omegaDot_{vector::zero},
  centreOfRotationName_
  {
    this->coeffs().template lookupOrDefault<word>
    (
      "centreOfRotationName",
      "centreOfRotation"
    )
  },
  centreOfRotation_{vector::zero}
{}


template<class CloudType>
mousse::NonInertialFrameForce<CloudType>::NonInertialFrameForce
(
  const NonInertialFrameForce& niff
)
:
  ParticleForce<CloudType>{niff},
  WName_{niff.WName_},
  W_{niff.W_},
  omegaName_{niff.omegaName_},
  omega_{niff.omega_},
  omegaDotName_{niff.omegaDotName_},
  omegaDot_{niff.omegaDot_},
  centreOfRotationName_{niff.centreOfRotationName_},
  centreOfRotation_{niff.centreOfRotation_}
{}


// Destructor 
template<class CloudType>
mousse::NonInertialFrameForce<CloudType>::~NonInertialFrameForce()
{}


// Member Functions 
template<class CloudType>
void mousse::NonInertialFrameForce<CloudType>::cacheFields(const bool store)
{
  W_ = vector::zero;
  omega_ = vector::zero;
  omegaDot_ = vector::zero;
  centreOfRotation_ = vector::zero;
  if (store) {
    if (this->mesh().template foundObject<uniformDimensionedVectorField>
        (
          WName_
        )) {
      uniformDimensionedVectorField W = this->mesh().template
        lookupObject<uniformDimensionedVectorField>(WName_);
      W_ = W.value();
    }
    if (this->mesh().template foundObject<uniformDimensionedVectorField>
        (
          omegaName_
        )) {
      uniformDimensionedVectorField omega = this->mesh().template
        lookupObject<uniformDimensionedVectorField>(omegaName_);
      omega_ = omega.value();
    }
    if (this->mesh().template foundObject<uniformDimensionedVectorField>
        (
          omegaDotName_
        )) {
      uniformDimensionedVectorField omegaDot = this->mesh().template
        lookupObject<uniformDimensionedVectorField>(omegaDotName_);
      omegaDot_ = omegaDot.value();
    }
    if (this->mesh().template foundObject<uniformDimensionedVectorField>
        (
          centreOfRotationName_
        )) {
      uniformDimensionedVectorField centreOfRotation =
        this->mesh().template
        lookupObject<uniformDimensionedVectorField>
        (
          centreOfRotationName_
        );
      centreOfRotation_ = centreOfRotation.value();
    }
  }
}


template<class CloudType>
mousse::forceSuSp mousse::NonInertialFrameForce<CloudType>::calcNonCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar /*Re*/,
  const scalar /*muc*/
) const
{
  forceSuSp value{vector::zero, 0.0};
  const vector r = p.position() - centreOfRotation_;
  value.Su() =
    mass*(-W_ + (r ^ omegaDot_) + 2.0*(p.U() ^ omega_) + (omega_ ^ (r ^ omega_)));
  return value;
}


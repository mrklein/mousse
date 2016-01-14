// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::
constantProperties()
:
  ParcelType::constantProperties(),
  youngsModulus_(this->dict_, 0.0),
  poissonsRatio_(this->dict_, 0.0)
{}
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties(cp),
  youngsModulus_(cp.youngsModulus_),
  poissonsRatio_(cp.poissonsRatio_)
{}
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties(parentDict),
  youngsModulus_(this->dict_, "youngsModulus"),
  poissonsRatio_(this->dict_, "poissonsRatio")
{}
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(owner, position, cellI, tetFaceI, tetPtI),
  f_(vector::zero),
  angularMomentum_(vector::zero),
  torque_(vector::zero),
  collisionRecords_()
{}
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& /*U0*/,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const typename ParcelType::constantProperties& constProps
)
:
  ParcelType
  (
    owner,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    torque0,
    constProps
  ),
  f_(f0),
  angularMomentum_(angularMomentum0),
  torque_(torque0),
  collisionRecords_()
{}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::CollidingParcel<ParcelType>::constantProperties::youngsModulus() const
{
  return youngsModulus_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::CollidingParcel<ParcelType>::constantProperties::poissonsRatio() const
{
  return poissonsRatio_.value();
}
// CollidingParcel Member Functions 
template<class ParcelType>
inline const mousse::vector& mousse::CollidingParcel<ParcelType>::f() const
{
  return f_;
}
template<class ParcelType>
inline const mousse::vector&
mousse::CollidingParcel<ParcelType>::angularMomentum() const
{
  return angularMomentum_;
}
template<class ParcelType>
inline const mousse::vector& mousse::CollidingParcel<ParcelType>::torque() const
{
  return torque_;
}
template<class ParcelType>
inline const mousse::collisionRecordList&
mousse::CollidingParcel<ParcelType>::collisionRecords() const
{
  return collisionRecords_;
}
template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::f()
{
  return f_;
}
template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::angularMomentum()
{
  return angularMomentum_;
}
template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::torque()
{
  return torque_;
}
template<class ParcelType>
inline mousse::collisionRecordList&
mousse::CollidingParcel<ParcelType>::collisionRecords()
{
  return collisionRecords_;
}
template<class ParcelType>
inline mousse::vector mousse::CollidingParcel<ParcelType>::omega() const
{
  return angularMomentum_/this->momentOfInertia();
}

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mathematical_constants.hpp"
using namespace mousse::constant::mathematical;
// Constructors 
template<class ParcelType>
inline
mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties()
:
  dict_(dictionary::null),
  parcelTypeId_(dict_, -1),
  rhoMin_(dict_, 0.0),
  rho0_(dict_, 0.0),
  minParcelMass_(dict_, 0.0)
{}
template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  dict_(cp.dict_),
  parcelTypeId_(cp.parcelTypeId_),
  rhoMin_(cp.rhoMin_),
  rho0_(cp.rho0_),
  minParcelMass_(cp.minParcelMass_)
{}
template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  dict_(parentDict.subOrEmptyDict("constantProperties")),
  parcelTypeId_(dict_, "parcelTypeId", -1),
  rhoMin_(dict_, "rhoMin", 1e-15),
  rho0_(dict_, "rho0"),
  minParcelMass_(dict_, "minParcelMass", 1e-15)
{}
template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::KinematicParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(owner, position, cellI, tetFaceI, tetPtI),
  active_(true),
  typeId_(-1),
  nParticle_(0),
  d_(0.0),
  dTarget_(0.0),
  U_(vector::zero),
  rho_(0.0),
  age_(0.0),
  tTurb_(0.0),
  UTurb_(vector::zero),
  rhoc_(0.0),
  Uc_(vector::zero),
  muc_(0.0)
{}
template<class ParcelType>
inline mousse::KinematicParcel<ParcelType>::KinematicParcel
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
  const vector& U0,
  const constantProperties& constProps
)
:
  ParcelType(owner, position, cellI, tetFaceI, tetPtI),
  active_(true),
  typeId_(typeId),
  nParticle_(nParticle0),
  d_(d0),
  dTarget_(dTarget0),
  U_(U0),
  rho_(constProps.rho0()),
  age_(0.0),
  tTurb_(0.0),
  UTurb_(vector::zero),
  rhoc_(0.0),
  Uc_(vector::zero),
  muc_(0.0)
{}
// constantProperties Member Functions
template<class ParcelType>
inline const mousse::dictionary&
mousse::KinematicParcel<ParcelType>::constantProperties::dict() const
{
  return dict_;
}
template<class ParcelType>
inline mousse::label
mousse::KinematicParcel<ParcelType>::constantProperties::parcelTypeId() const
{
  return parcelTypeId_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::rhoMin() const
{
  return rhoMin_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::rho0() const
{
  return rho0_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::KinematicParcel<ParcelType>::constantProperties::minParcelMass() const
{
  return minParcelMass_.value();
}
// KinematicParcel Member Functions 
template<class ParcelType>
inline bool mousse::KinematicParcel<ParcelType>::active() const
{
  return active_;
}
template<class ParcelType>
inline mousse::label mousse::KinematicParcel<ParcelType>::typeId() const
{
  return typeId_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::nParticle() const
{
  return nParticle_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::d() const
{
  return d_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::dTarget() const
{
  return dTarget_;
}
template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::U() const
{
  return U_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::rho() const
{
  return rho_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::age() const
{
  return age_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::tTurb() const
{
  return tTurb_;
}
template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::UTurb() const
{
  return UTurb_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::rhoc() const
{
  return rhoc_;
}
template<class ParcelType>
inline const mousse::vector& mousse::KinematicParcel<ParcelType>::Uc() const
{
  return Uc_;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::muc() const
{
  return muc_;
}
template<class ParcelType>
inline bool& mousse::KinematicParcel<ParcelType>::active()
{
  return active_;
}
template<class ParcelType>
inline mousse::label& mousse::KinematicParcel<ParcelType>::typeId()
{
  return typeId_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::nParticle()
{
  return nParticle_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::d()
{
  return d_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::dTarget()
{
  return dTarget_;
}
template<class ParcelType>
inline mousse::vector& mousse::KinematicParcel<ParcelType>::U()
{
  return U_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::rho()
{
  return rho_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::age()
{
  return age_;
}
template<class ParcelType>
inline mousse::scalar& mousse::KinematicParcel<ParcelType>::tTurb()
{
  return tTurb_;
}
template<class ParcelType>
inline mousse::vector& mousse::KinematicParcel<ParcelType>::UTurb()
{
  return UTurb_;
}
template<class ParcelType>
inline mousse::label mousse::KinematicParcel<ParcelType>::faceInterpolation() const
{
  // Use volume-based interpolation if dealing with external faces
  if (this->cloud().internalFace(this->face()))
  {
    return this->face();
  }
  else
  {
    return -1;
  }
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::massCell
(
  const label cellI
) const
{
  return rhoc_*this->mesh().cellVolumes()[cellI];
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::mass() const
{
  return rho_*volume();
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::momentOfInertia() const
{
  return 0.1*mass()*sqr(d_);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::volume() const
{
  return volume(d_);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::volume(const scalar d)
{
  return pi/6.0*pow3(d);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaP() const
{
  return areaP(d_);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaP(const scalar d)
{
  return 0.25*areaS(d);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaS() const
{
  return areaS(d_);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::areaS(const scalar d)
{
  return pi*d*d;
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::Re
(
  const vector& U,
  const scalar d,
  const scalar rhoc,
  const scalar muc
) const
{
  return rhoc*mag(U - Uc_)*d/(muc + ROOTVSMALL);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::We
(
  const vector& U,
  const scalar d,
  const scalar rhoc,
  const scalar sigma
) const
{
  return rhoc*magSqr(U - Uc_)*d/(sigma + ROOTVSMALL);
}
template<class ParcelType>
inline mousse::scalar mousse::KinematicParcel<ParcelType>::Eo
(
  const vector& a,
  const scalar d,
  const scalar sigma
) const
{
  vector dir = U_/(mag(U_) + ROOTVSMALL);
  return mag(a & dir)*(rho_ - rhoc_)*sqr(d)/(sigma + ROOTVSMALL);
}

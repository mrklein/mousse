// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ParcelType>
inline
mousse::ReactingParcel<ParcelType>::constantProperties::constantProperties()
:
  ParcelType::constantProperties(),
  pMin_(this->dict_, 0.0),
  constantVolume_(this->dict_, false)
{}
template<class ParcelType>
inline mousse::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties(cp),
  pMin_(cp.pMin_),
  constantVolume_(cp.constantVolume_)
{}
template<class ParcelType>
inline mousse::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties(parentDict),
  pMin_(this->dict_, "pMin", 1000.0),
  constantVolume_(this->dict_, word("constantVolume"))
{}
template<class ParcelType>
inline mousse::ReactingParcel<ParcelType>::ReactingParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
  mass0_(0.0),
  Y_(0),
  pc_(0.0)
{}
template<class ParcelType>
inline mousse::ReactingParcel<ParcelType>::ReactingParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const scalarField& Y0,
  const constantProperties& constProps
)
:
  ParcelType
  (
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    f0,
    angularMomentum0,
    torque0,
    constProps
  ),
  mass0_(0.0),
  Y_(Y0),
  pc_(0.0)
{
  // Set initial parcel mass
  mass0_ = this->mass();
}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::ReactingParcel<ParcelType>::constantProperties::pMin() const
{
  return pMin_.value();
}
template<class ParcelType>
inline bool
mousse::ReactingParcel<ParcelType>::constantProperties::constantVolume() const
{
  return constantVolume_.value();
}
// ThermoParcel Member Functions
template<class ParcelType>
inline mousse::scalar mousse::ReactingParcel<ParcelType>::mass0() const
{
  return mass0_;
}
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingParcel<ParcelType>::Y() const
{
  return Y_;
}
template<class ParcelType>
inline mousse::scalar mousse::ReactingParcel<ParcelType>::pc() const
{
  return pc_;
}
template<class ParcelType>
inline mousse::scalar& mousse::ReactingParcel<ParcelType>::pc()
{
  return pc_;
}
template<class ParcelType>
inline mousse::scalar& mousse::ReactingParcel<ParcelType>::mass0()
{
  return mass0_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingParcel<ParcelType>::Y()
{
  return Y_;
}

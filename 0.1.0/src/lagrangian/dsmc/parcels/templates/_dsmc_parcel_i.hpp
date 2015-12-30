// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mathematical_constants.hpp"
// Constructors 
template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::constantProperties::constantProperties()
:
  mass_(0),
  d_(0)
{}
template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& dict
)
:
  mass_(readScalar(dict.lookup("mass"))),
  d_(readScalar(dict.lookup("diameter"))),
  internalDegreesOfFreedom_
  (
    readInt(dict.lookup("internalDegreesOfFreedom"))
  ),
  omega_(readScalar(dict.lookup("omega")))
{}
template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::DSMCParcel
(
  const polyMesh& mesh,
  const vector& position,
  const vector& U,
  const scalar Ei,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId
)
:
  ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
  U_(U),
  Ei_(Ei),
  typeId_(typeId)
{}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::mass() const
{
  return mass_;
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCParcel<ParcelType>::constantProperties::d() const
{
  return d_;
}
template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::sigmaT() const
{
  return constant::mathematical::pi*d_*d_;
}
template<class ParcelType>
inline mousse::direction
mousse::DSMCParcel<ParcelType>::constantProperties::internalDegreesOfFreedom()
const
{
  return internalDegreesOfFreedom_;
}
template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::omega() const
{
  return omega_;
}
// DSMCParcel Member Functions 
template<class ParcelType>
inline mousse::label mousse::DSMCParcel<ParcelType>::typeId() const
{
  return typeId_;
}
template<class ParcelType>
inline const mousse::vector& mousse::DSMCParcel<ParcelType>::U() const
{
  return U_;
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCParcel<ParcelType>::Ei() const
{
  return Ei_;
}
template<class ParcelType>
inline mousse::vector& mousse::DSMCParcel<ParcelType>::U()
{
  return U_;
}
template<class ParcelType>
inline mousse::scalar& mousse::DSMCParcel<ParcelType>::Ei()
{
  return Ei_;
}

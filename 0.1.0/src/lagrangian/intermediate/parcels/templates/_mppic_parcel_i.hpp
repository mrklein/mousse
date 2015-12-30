// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ParcelType>
inline mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(owner, position, cellI, tetFaceI, tetPtI),
  UCorrect_(vector::zero)
{}
template<class ParcelType>
inline mousse::MPPICParcel<ParcelType>::MPPICParcel
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
  const vector& UCorrect0,
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
    U0,
    constProps
  ),
  UCorrect_(UCorrect0)
{}
// Member Functions 
template<class ParcelType>
inline const mousse::vector& mousse::MPPICParcel<ParcelType>::UCorrect() const
{
  return UCorrect_;
}
template<class ParcelType>
inline mousse::vector& mousse::MPPICParcel<ParcelType>::UCorrect()
{
  return UCorrect_;
}

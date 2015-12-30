// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::vector& mousse::NonInertialFrameForce<CloudType>::W() const
{
  return W_;
}
template<class CloudType>
inline const mousse::vector& mousse::NonInertialFrameForce<CloudType>::omega() const
{
  return omega_;
}
template<class CloudType>
inline const mousse::vector&
mousse::NonInertialFrameForce<CloudType>::omegaDot() const
{
  return omegaDot_;
}
template<class CloudType>
inline const mousse::vector&
mousse::NonInertialFrameForce<CloudType>::centreOfRotation() const
{
  return centreOfRotation_;
}

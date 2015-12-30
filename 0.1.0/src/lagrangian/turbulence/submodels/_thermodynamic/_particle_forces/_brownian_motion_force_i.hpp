// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline mousse::scalar mousse::BrownianMotionForce<CloudType>::lambda() const
{
  return lambda_;
}
template<class CloudType>
inline bool mousse::BrownianMotionForce<CloudType>::turbulence() const
{
  return turbulence_;
}

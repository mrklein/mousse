// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const CloudType& mousse::ParticleForceList<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
inline CloudType& mousse::ParticleForceList<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
inline const mousse::fvMesh& mousse::ParticleForceList<CloudType>::mesh() const
{
  return mesh_;
}
template<class CloudType>
inline const mousse::dictionary& mousse::ParticleForceList<CloudType>::dict() const
{
  return dict_;
}
template<class CloudType>
inline void mousse::ParticleForceList<CloudType>::setCalcCoupled(bool flag)
{
  calcCoupled_ = flag;
}
template<class CloudType>
inline void mousse::ParticleForceList<CloudType>::setCalcNonCoupled(bool flag)
{
  calcNonCoupled_ = flag;
}

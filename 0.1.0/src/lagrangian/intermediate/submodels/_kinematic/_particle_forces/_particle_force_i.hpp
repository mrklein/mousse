// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const CloudType& mousse::ParticleForce<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
inline CloudType& mousse::ParticleForce<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
inline const mousse::fvMesh& mousse::ParticleForce<CloudType>::mesh() const
{
  return mesh_;
}
template<class CloudType>
inline const mousse::dictionary& mousse::ParticleForce<CloudType>::coeffs() const
{
  return coeffs_;
}

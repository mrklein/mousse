// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::Switch&
mousse::ParticleCollector<CloudType>::resetOnWrite() const
{
  return resetOnWrite_;
}

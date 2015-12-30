// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::vector& mousse::GravityForce<CloudType>::g() const
{
  return g_;
}

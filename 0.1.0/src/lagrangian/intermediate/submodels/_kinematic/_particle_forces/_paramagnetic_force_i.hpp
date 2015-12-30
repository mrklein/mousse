// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::word&
mousse::ParamagneticForce<CloudType>::HdotGradHName() const
{
  return HdotGradHName_;
}
template<class CloudType>
inline mousse::scalar
mousse::ParamagneticForce<CloudType>::magneticSusceptibility() const
{
  return magneticSusceptibility_;
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::Switch&
mousse::FacePostProcessing<CloudType>::resetOnWrite() const
{
  return resetOnWrite_;
}

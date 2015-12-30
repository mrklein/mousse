// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
mousse::label mousse::PatchPostProcessing<CloudType>::maxStoredParcels() const
{
  return maxStoredParcels_;
}
template<class CloudType>
const mousse::labelList& mousse::PatchPostProcessing<CloudType>::patchIDs() const
{
  return patchIDs_;
}

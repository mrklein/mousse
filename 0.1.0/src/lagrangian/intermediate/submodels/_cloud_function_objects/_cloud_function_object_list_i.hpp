// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const CloudType& mousse::CloudFunctionObjectList<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
inline CloudType& mousse::CloudFunctionObjectList<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
inline const mousse::dictionary&
mousse::CloudFunctionObjectList<CloudType>::dict() const
{
  return dict_;
}

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class CloudType>
inline const mousse::MPPICCloud<CloudType>&
mousse::MPPICCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const mousse::PackingModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::packingModel() const
{
  return packingModel_();
}
template<class CloudType>
inline mousse::PackingModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::packingModel()
{
  return packingModel_();
}
template<class CloudType>
inline const mousse::DampingModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::dampingModel() const
{
  return dampingModel_();
}
template<class CloudType>
inline mousse::DampingModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::dampingModel()
{
  return dampingModel_();
}
template<class CloudType>
inline const mousse::IsotropyModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::isotropyModel() const
{
  return isotropyModel_();
}
template<class CloudType>
inline mousse::IsotropyModel<mousse::MPPICCloud<CloudType> >&
mousse::MPPICCloud<CloudType>::isotropyModel()
{
  return isotropyModel_();
}

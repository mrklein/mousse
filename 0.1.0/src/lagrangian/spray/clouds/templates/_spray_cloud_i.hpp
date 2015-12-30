// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class CloudType>
inline const mousse::SprayCloud<CloudType>&
mousse::SprayCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const mousse::AtomizationModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::atomization() const
{
  return atomizationModel_;
}
template<class CloudType>
inline mousse::AtomizationModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::atomization()
{
  return atomizationModel_();
}
template<class CloudType>
inline const mousse::BreakupModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::breakup() const
{
  return breakupModel_;
}
template<class CloudType>
inline mousse::BreakupModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::breakup()
{
  return breakupModel_();
}
template<class CloudType>
inline mousse::scalar mousse::SprayCloud<CloudType>::averageParcelMass() const
{
  return averageParcelMass_;
}

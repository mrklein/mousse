// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Member Functions 
template<class CompType, class ThermoType>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::chemistryModel<CompType, ThermoType>::RR()
{
  return RR_;
}
template<class CompType, class ThermoType>
inline const mousse::PtrList<mousse::Reaction<ThermoType> >&
mousse::chemistryModel<CompType, ThermoType>::reactions() const
{
  return reactions_;
}
template<class CompType, class ThermoType>
inline const mousse::PtrList<ThermoType>&
mousse::chemistryModel<CompType, ThermoType>::specieThermo() const
{
  return specieThermo_;
}
template<class CompType, class ThermoType>
inline mousse::label
mousse::chemistryModel<CompType, ThermoType>::nSpecie() const
{
  return nSpecie_;
}
template<class CompType, class ThermoType>
inline mousse::label
mousse::chemistryModel<CompType, ThermoType>::nReaction() const
{
  return nReaction_;
}
template<class CompType, class ThermoType>
inline mousse::scalar
mousse::chemistryModel<CompType, ThermoType>::Treact() const
{
  return Treact_;
}
template<class CompType, class ThermoType>
inline mousse::scalar&
mousse::chemistryModel<CompType, ThermoType>::Treact()
{
  return Treact_;
}
template<class CompType, class ThermoType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::chemistryModel<CompType, ThermoType>::RR
(
  const label i
) const
{
  return RR_[i];
}
template<class CompType, class ThermoType>
mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::chemistryModel<CompType, ThermoType>::RR
(
  const label i
)
{
  return RR_[i];
}

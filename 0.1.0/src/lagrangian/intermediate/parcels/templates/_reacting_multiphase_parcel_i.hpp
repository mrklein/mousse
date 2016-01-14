// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties()
:
  ParcelType::constantProperties(),
  TDevol_(this->dict_, 0.0),
  LDevol_(this->dict_, 0.0),
  hRetentionCoeff_(this->dict_, 0.0)
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties(cp),
  TDevol_(cp.TDevol_),
  LDevol_(cp.LDevol_),
  hRetentionCoeff_(cp.hRetentionCoeff_)
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties(parentDict),
  TDevol_(this->dict_, "TDevol"),
  LDevol_(this->dict_, "LDevol"),
  hRetentionCoeff_(this->dict_, "hRetentionCoeff")
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
  YGas_(0),
  YLiquid_(0),
  YSolid_(0),
  canCombust_(0)
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const scalarField& Y0,
  const scalarField& YGas0,
  const scalarField& YLiquid0,
  const scalarField& YSolid0,
  const constantProperties& constProps
)
:
  ParcelType
  (
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    f0,
    angularMomentum0,
    torque0,
    Y0,
    constProps
  ),
  YGas_(YGas0),
  YLiquid_(YLiquid0),
  YSolid_(YSolid0),
  canCombust_(0)
{}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::TDevol() const
{
  return TDevol_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::LDevol() const
{
  return LDevol_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
hRetentionCoeff() const
{
  scalar value = hRetentionCoeff_.value();
  if ((value < 0) || (value > 1))
  {
    FATAL_ERROR_IN
    (
      "ReactingMultiphaseParcel<ParcelType>::constantProperties::"
      "constantProperties"
    )   << "hRetentionCoeff must be in the range 0 to 1" << nl
      << exit(FatalError) << endl;
  }
  return value;
}
// ThermoParcel Member Functions
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YGas() const
{
  return YGas_;
}
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YLiquid() const
{
  return YLiquid_;
}
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YSolid() const
{
  return YSolid_;
}
template<class ParcelType>
inline mousse::label
mousse::ReactingMultiphaseParcel<ParcelType>::canCombust() const
{
  return canCombust_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YGas()
{
  return YGas_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YLiquid()
{
  return YLiquid_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YSolid()
{
  return YSolid_;
}
template<class ParcelType>
inline mousse::label& mousse::ReactingMultiphaseParcel<ParcelType>::canCombust()
{
  return canCombust_;
}

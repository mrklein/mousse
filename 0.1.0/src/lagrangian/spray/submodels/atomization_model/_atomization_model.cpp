// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_atomization_model.hpp"

// Constructors
template<class CloudType>
mousse::AtomizationModel<CloudType>::AtomizationModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>{owner}
{}
template<class CloudType>
mousse::AtomizationModel<CloudType>::AtomizationModel
(
  const AtomizationModel<CloudType>& am
)
:
  CloudSubModelBase<CloudType>{am}
{}
template<class CloudType>
mousse::AtomizationModel<CloudType>::AtomizationModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type}
{}
// Destructor
template<class CloudType>
mousse::AtomizationModel<CloudType>::~AtomizationModel()
{}
// Member Functions
template<class CloudType>
mousse::scalar mousse::AtomizationModel<CloudType>::Taverage
(
  const scalar& Tl,
  const scalar& Tc
) const
{
  return (2.0*Tl + Tc)/3.0;
}
#include "_atomization_model_new.cpp"

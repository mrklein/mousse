// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_dispersion_model.hpp"
// Constructors 
template<class CloudType>
mousse::DispersionModel<CloudType>::DispersionModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>(owner)
{}
template<class CloudType>
mousse::DispersionModel<CloudType>::DispersionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type)
{}
template<class CloudType>
mousse::DispersionModel<CloudType>::DispersionModel
(
  const DispersionModel<CloudType>& dm
)
:
  CloudSubModelBase<CloudType>(dm)
{}
// Destructor 
template<class CloudType>
mousse::DispersionModel<CloudType>::~DispersionModel()
{}
#include "_dispersion_model_new.cpp"

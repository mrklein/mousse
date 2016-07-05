// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_composition.hpp"


// Constructors 
template<class CloudType>
mousse::NoComposition<CloudType>::NoComposition
(
  const dictionary&,
  CloudType& owner
)
:
  CompositionModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoComposition<CloudType>::NoComposition
(
  const NoComposition<CloudType>& cm
)
:
  CompositionModel<CloudType>{cm}
{}


// Destructor 
template<class CloudType>
mousse::NoComposition<CloudType>::~NoComposition()
{}


// Member Functions 
template<class CloudType>
const mousse::scalarField& mousse::NoComposition<CloudType>::YMixture0() const
{
  return scalarField::null();
}


template<class CloudType>
mousse::label mousse::NoComposition<CloudType>::idGas() const
{
  return -1;
}


template<class CloudType>
mousse::label mousse::NoComposition<CloudType>::idLiquid() const
{
  return -1;
}


template<class CloudType>
mousse::label mousse::NoComposition<CloudType>::idSolid() const
{
  return -1;
}


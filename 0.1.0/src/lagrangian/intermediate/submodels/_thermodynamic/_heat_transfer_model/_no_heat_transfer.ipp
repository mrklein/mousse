// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_heat_transfer.hpp"


// Constructors 
template<class CloudType>
mousse::NoHeatTransfer<CloudType>::NoHeatTransfer
(
  const dictionary&,
  CloudType& owner
)
:
  HeatTransferModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoHeatTransfer<CloudType>::NoHeatTransfer
(
  const NoHeatTransfer<CloudType>& htm
)
:
  HeatTransferModel<CloudType>{htm.owner_}
{}


// Destructor 
template<class CloudType>
mousse::NoHeatTransfer<CloudType>::~NoHeatTransfer()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoHeatTransfer<CloudType>::active() const
{
  return false;
}


template<class CloudType>
mousse::scalar mousse::NoHeatTransfer<CloudType>::Nu
(
  const scalar,
  const scalar
) const
{
  return 0.0;
}


template<class CloudType>
mousse::scalar mousse::NoHeatTransfer<CloudType>::Pr() const
{
  return 1.0;
}


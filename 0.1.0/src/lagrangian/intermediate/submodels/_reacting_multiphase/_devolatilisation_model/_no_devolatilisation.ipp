// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_devolatilisation.hpp"


// Constructors 
template<class CloudType>
mousse::NoDevolatilisation<CloudType>::NoDevolatilisation
(
  const dictionary&,
  CloudType& owner
)
:
  DevolatilisationModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoDevolatilisation<CloudType>::NoDevolatilisation
(
  const NoDevolatilisation<CloudType>& dm
)
:
  DevolatilisationModel<CloudType>{dm.owner_}
{}


// Destructor 
template<class CloudType>
mousse::NoDevolatilisation<CloudType>::~NoDevolatilisation()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoDevolatilisation<CloudType>::active() const
{
  return false;
}


template<class CloudType>
void mousse::NoDevolatilisation<CloudType>::calculate
(
  const scalar,
  const scalar,
  const scalar,
  const scalar,
  const scalar,
  const scalarField&,
  const scalarField&,
  const scalarField&,
  label& canCombust,
  scalarField&
) const
{
  // Model does not stop combustion taking place
  canCombust = true;
}


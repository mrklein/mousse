// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_phase_change.hpp"


// Constructors 
template<class CloudType>
mousse::NoPhaseChange<CloudType>::NoPhaseChange
(
  const dictionary&,
  CloudType& owner
)
:
  PhaseChangeModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoPhaseChange<CloudType>::NoPhaseChange
(
  const NoPhaseChange<CloudType>& pcm
)
:
  PhaseChangeModel<CloudType>{pcm.owner_}
{}


// Destructor 
template<class CloudType>
mousse::NoPhaseChange<CloudType>::~NoPhaseChange()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoPhaseChange<CloudType>::active() const
{
  return false;
}


template<class CloudType>
void mousse::NoPhaseChange<CloudType>::calculate
(
  const scalar /*dt*/,
  const label /*cellI*/,
  const scalar /*Re*/,
  const scalar /*Pr*/,
  const scalar /*d*/,
  const scalar /*nu*/,
  const scalar /*T*/,
  const scalar /*Ts*/,
  const scalar /*pc*/,
  const scalar /*Tc*/,
  const scalarField& /*X*/,
  scalarField& /*dMassPC*/
) const
{
  // Nothing to do...
}


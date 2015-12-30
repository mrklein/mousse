// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_surface_reaction.hpp"
// Constructors 
template<class CloudType>
mousse::NoSurfaceReaction<CloudType>::NoSurfaceReaction
(
  const dictionary&,
  CloudType& owner
)
:
  SurfaceReactionModel<CloudType>(owner)
{}
template<class CloudType>
mousse::NoSurfaceReaction<CloudType>::NoSurfaceReaction
(
  const NoSurfaceReaction<CloudType>& srm
)
:
  SurfaceReactionModel<CloudType>(srm.owner_)
{}
// Destructor 
template<class CloudType>
mousse::NoSurfaceReaction<CloudType>::~NoSurfaceReaction()
{}
// Member Functions 
template<class CloudType>
bool mousse::NoSurfaceReaction<CloudType>::active() const
{
  return false;
}
template<class CloudType>
mousse::scalar mousse::NoSurfaceReaction<CloudType>::calculate
(
  const scalar,
  const label,
  const scalar,
  const scalar,
  const scalar,
  const scalar,
  const scalar,
  const scalar,
  const scalarField&,
  const scalarField&,
  const scalarField&,
  const scalarField&,
  const scalar,
  scalarField&,
  scalarField&,
  scalarField&,
  scalarField&
) const
{
  // do nothing
  return 0.0;
}

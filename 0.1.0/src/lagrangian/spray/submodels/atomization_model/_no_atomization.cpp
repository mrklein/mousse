// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_atomization.hpp"

// Constructors
template<class CloudType>
mousse::NoAtomization<CloudType>::NoAtomization
(
  const dictionary&,
  CloudType& owner
)
:
  AtomizationModel<CloudType>(owner)
{}
template<class CloudType>
mousse::NoAtomization<CloudType>::NoAtomization
(
  const NoAtomization<CloudType>& am
)
:
  AtomizationModel<CloudType>(am)
{}
// Destructor
template<class CloudType>
mousse::NoAtomization<CloudType>::~NoAtomization()
{}
// Member Functions
template<class CloudType>
bool mousse::NoAtomization<CloudType>::active() const
{
  return false;
}
template<class CloudType>
mousse::scalar mousse::NoAtomization<CloudType>::initLiquidCore() const
{
  return 0.0;
}
template<class CloudType>
bool mousse::NoAtomization<CloudType>::calcChi() const
{
  return false;
}
template<class CloudType>
void mousse::NoAtomization<CloudType>::update
(
  const scalar /*dt*/,
  scalar& /*d*/,
  scalar& /*liquidCore*/,
  scalar& /*tc*/,
  const scalar /*rho*/,
  const scalar /*mu*/,
  const scalar /*sigma*/,
  const scalar /*volFlowRate*/,
  const scalar /*rhoAv*/,
  const scalar /*Urel*/,
  const vector& /*pos*/,
  const vector& /*injectionPos*/,
  const scalar /*pAmbient*/,
  const scalar /*chi*/,
  cachedRandom& /*rndGen*/
) const
{}

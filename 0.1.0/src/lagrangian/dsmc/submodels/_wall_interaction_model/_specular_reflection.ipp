// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "_specular_reflection.hpp"


// Constructors
template<class CloudType>
mousse::SpecularReflection<CloudType>::SpecularReflection
(
  const dictionary&,
  CloudType& cloud
)
:
  WallInteractionModel<CloudType>{cloud}
{}


// Destructor
template<class CloudType>
mousse::SpecularReflection<CloudType>::~SpecularReflection()
{}


// Member Functions
template<class CloudType>
void mousse::SpecularReflection<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const wallPolyPatch&
)
{
  vector& U = p.U();
  vector nw = p.normal();
  nw /= mag(nw);
  scalar U_dot_nw = U & nw;
  if (U_dot_nw > 0.0) {
    U -= 2.0*U_dot_nw*nw;
  }
}


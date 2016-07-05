#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_COLLIDING_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_COLLIDING_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicCollidingParcel
// Description
//   Definition of basic kinematic colliding parcel

#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_colliding_parcel.hpp"


namespace mousse {

typedef CollidingParcel<KinematicParcel<particle>>
  basicKinematicCollidingParcel;

template<>
inline bool contiguous<basicKinematicCollidingParcel>()
{
  return true;
}

}

#endif


#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicParcel
// Description
//   Definition of basic kinematic parcel

#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"


namespace mousse {

typedef KinematicParcel<particle> basicKinematicParcel;

template<>
inline bool contiguous<basicKinematicParcel>()
{
  return true;
}

}

#endif


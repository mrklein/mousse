#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_MPPIC_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_KINEMATIC_MPPIC_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicMPPICParcel
// Description
//   Definition of basic kinematic MPPIC parcel

#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_mppic_parcel.hpp"


namespace mousse {

typedef MPPICParcel<KinematicParcel<particle>> basicKinematicMPPICParcel;

template<>
inline bool contiguous<basicKinematicMPPICParcel>()
{
  return true;
}

}

#endif


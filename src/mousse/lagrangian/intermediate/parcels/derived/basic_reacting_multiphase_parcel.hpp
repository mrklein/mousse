#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_REACTING_MULTIPHASE_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_REACTING_MULTIPHASE_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicReactingMultiphaseParcel
// Description
//   Definition of basic reacting parcel

#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"
#include "_reacting_parcel.hpp"
#include "_reacting_multiphase_parcel.hpp"


namespace mousse {

typedef ReactingMultiphaseParcel
<
  ReactingParcel
  <
    ThermoParcel
    <
      KinematicParcel
      <
        particle
      >
    >
  >
> basicReactingMultiphaseParcel;

template<>
inline bool contiguous<basicReactingMultiphaseParcel>()
{
  return false;
}

}

#endif


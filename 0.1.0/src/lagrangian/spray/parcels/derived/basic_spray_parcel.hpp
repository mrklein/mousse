#ifndef LAGRANGIAN_SPRAY_PARCELS_DERIVED_BASIC_SPRAY_PARCEL_HPP_
#define LAGRANGIAN_SPRAY_PARCELS_DERIVED_BASIC_SPRAY_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSprayParcel
// Description
//   Definition of spray parcel
// SourceFiles
//   basic_spray_parcel.cpp
//   basic_spray_parcel_io.cpp
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"
#include "_reacting_parcel.hpp"
#include "_spray_parcel.hpp"
namespace mousse
{
  typedef SprayParcel
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
  > basicSprayParcel;
  template<>
  inline bool contiguous<basicSprayParcel>()
  {
    return false;
  }
}
#endif

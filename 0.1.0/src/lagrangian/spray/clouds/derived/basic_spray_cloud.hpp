#ifndef LAGRANGIAN_SPRAY_CLOUDS_DERIVED_BASIC_SPRAY_CLOUD_HPP_
#define LAGRANGIAN_SPRAY_CLOUDS_DERIVED_BASIC_SPRAY_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSprayCloud
// Description
//   Cloud class to introduce reacting spray parcels

#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_thermo_cloud.hpp"
#include "_reacting_cloud.hpp"
#include "_spray_cloud.hpp"
#include "basic_spray_parcel.hpp"


namespace mousse {

typedef SprayCloud
<
  ReactingCloud
  <
    ThermoCloud
    <
      KinematicCloud
      <
        Cloud
        <
          basicSprayParcel
        >
      >
    >
  >
> basicSprayCloud;

}

#endif


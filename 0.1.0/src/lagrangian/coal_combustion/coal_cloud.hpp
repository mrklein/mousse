#ifndef LAGRANGIAN_COAL_COMBUSTION_COAL_CLOUD_HPP_
#define LAGRANGIAN_COAL_COMBUSTION_COAL_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coalCloud
// Description
//   Cloud class to introduce coal parcels

#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_thermo_cloud.hpp"
#include "_reacting_cloud.hpp"
#include "_reacting_multiphase_cloud.hpp"
#include "coal_parcel.hpp"


namespace mousse {

typedef ReactingMultiphaseCloud
<
  ReactingCloud
  <
    ThermoCloud
    <
      KinematicCloud
      <
        Cloud
        <
          coalParcel
        >
      >
    >
  >
> coalCloud;

}

#endif


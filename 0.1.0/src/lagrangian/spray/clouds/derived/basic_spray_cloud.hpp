// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSprayCloud
// Description
//   Cloud class to introduce reacting spray parcels
#ifndef basic_spray_cloud_hpp_
#define basic_spray_cloud_hpp_
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_thermo_cloud.hpp"
#include "_reacting_cloud.hpp"
#include "_spray_cloud.hpp"
#include "basic_spray_parcel.hpp"
namespace mousse
{
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

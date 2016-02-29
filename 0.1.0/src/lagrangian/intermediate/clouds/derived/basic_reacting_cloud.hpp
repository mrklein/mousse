#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_REACTING_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_REACTING_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicReactingCloud
// Description
//   Cloud class to introduce reacting parcels
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_thermo_cloud.hpp"
#include "_reacting_cloud.hpp"
#include "basic_reacting_parcel.hpp"
namespace mousse
{
  typedef ReactingCloud
  <
    ThermoCloud
    <
      KinematicCloud
      <
        Cloud
        <
          basicReactingParcel
        >
      >
    >
  > basicReactingCloud;
}
#endif

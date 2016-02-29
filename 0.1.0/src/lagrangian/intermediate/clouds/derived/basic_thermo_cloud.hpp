#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_THERMO_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_THERMO_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicThermoCloud
// Description
//   Cloud class to introduce thermodynamic parcels
#include "_thermo_cloud.hpp"
#include "basic_thermo_parcel.hpp"
namespace mousse
{
  typedef ThermoCloud
  <
    KinematicCloud
    <
      Cloud
      <
        basicThermoParcel
      >
    >
  > basicThermoCloud;
}
#endif

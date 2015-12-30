// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicThermoCloud
// Description
//   Cloud class to introduce thermodynamic parcels
#ifndef basic_thermo_cloud_hpp_
#define basic_thermo_cloud_hpp_
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

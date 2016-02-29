#ifndef LAGRANGIAN_COAL_COMBUSTION_COAL_PARCEL_HPP_
#define LAGRANGIAN_COAL_COMBUSTION_COAL_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coalParcel
// Description
//   Definition of coal parcel
// SourceFiles
//   coal_parcel.cpp
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"
#include "_reacting_parcel.hpp"
#include "_reacting_multiphase_parcel.hpp"
namespace mousse
{
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
  > coalParcel;
  template<>
  inline bool contiguous<coalParcel>()
  {
    return false;
  }
}
#endif

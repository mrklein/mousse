#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_REACTING_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_REACTING_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicReactingParcel
// Description
//   Definition of basic reacting parcel
// SourceFiles
//   basic_reacting_parcel.cpp
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"
#include "_reacting_parcel.hpp"
namespace mousse
{
  typedef ReactingParcel<ThermoParcel<KinematicParcel<particle> > >
    basicReactingParcel;
  template<>
  inline bool contiguous<basicReactingParcel>()
  {
    return false;
  }
}
#endif

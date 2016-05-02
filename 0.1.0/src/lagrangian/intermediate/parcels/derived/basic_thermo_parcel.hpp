#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_THERMO_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_DERIVED_BASIC_THERMO_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicThermoParcel
// Description
//   Definition of basic thermo parcel

#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"


namespace mousse {

typedef ThermoParcel<KinematicParcel<particle>> basicThermoParcel;

template<> inline bool contiguous<basicThermoParcel>() { return true; }

}

#endif


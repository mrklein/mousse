// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicThermoParcel
// Description
//   Definition of basic thermo parcel
// SourceFiles
//   basic_thermo_parcel.cpp
#ifndef basic_thermo_parcel_hpp_
#define basic_thermo_parcel_hpp_
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_thermo_parcel.hpp"
namespace mousse
{
  typedef ThermoParcel<KinematicParcel<particle> > basicThermoParcel;
  template<>
  inline bool contiguous<basicThermoParcel>()
  {
    return true;
  }
}
#endif

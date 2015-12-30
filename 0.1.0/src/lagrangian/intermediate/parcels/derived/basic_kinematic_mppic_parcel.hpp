// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicMPPICParcel
// Description
//   Definition of basic kinematic MPPIC parcel
// SourceFiles
//   basic_kinematic_mppic_parcel.hpp
#ifndef basic_kinematic_mppic_parcel_hpp_
#define basic_kinematic_mppic_parcel_hpp_
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_mppic_parcel.hpp"
namespace mousse
{
  typedef MPPICParcel<KinematicParcel<particle> > basicKinematicMPPICParcel;
  template<>
  inline bool contiguous<basicKinematicMPPICParcel>()
  {
    return true;
  }
}
#endif

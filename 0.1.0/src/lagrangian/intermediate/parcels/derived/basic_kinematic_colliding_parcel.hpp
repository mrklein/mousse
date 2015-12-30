// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicCollidingParcel
// Description
//   Definition of basic kinematic colliding parcel
// SourceFiles
//   basic_kinematic_parcel.hpp
#ifndef basic_kinematic_colliding_parcel_hpp_
#define basic_kinematic_colliding_parcel_hpp_
#include "contiguous.hpp"
#include "particle.hpp"
#include "_kinematic_parcel.hpp"
#include "_colliding_parcel.hpp"
namespace mousse
{
  typedef CollidingParcel<KinematicParcel<particle> >
    basicKinematicCollidingParcel;
  template<>
  inline bool contiguous<basicKinematicCollidingParcel>()
  {
    return true;
  }
}
#endif

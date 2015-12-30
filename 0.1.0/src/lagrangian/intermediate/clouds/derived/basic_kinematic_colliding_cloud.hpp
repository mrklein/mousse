// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicCollidingCloud
// Description
//   Cloud class to introduce kinematic colliding parcels
#ifndef basic_kinematic_colliding_cloud_hpp_
#define basic_kinematic_colliding_cloud_hpp_
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_colliding_cloud.hpp"
#include "basic_kinematic_colliding_parcel.hpp"
namespace mousse
{
  typedef CollidingCloud
  <
    KinematicCloud
    <
      Cloud
      <
        basicKinematicCollidingParcel
      >
    >
  > basicKinematicCollidingCloud;
}
#endif

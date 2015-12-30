// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicCloud
// Description
//   Cloud class to introduce kinematic parcels
#ifndef basic_kinematic_cloud_hpp_
#define basic_kinematic_cloud_hpp_
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "basic_kinematic_parcel.hpp"
namespace mousse
{
  typedef KinematicCloud<Cloud<basicKinematicParcel> > basicKinematicCloud;
}
#endif

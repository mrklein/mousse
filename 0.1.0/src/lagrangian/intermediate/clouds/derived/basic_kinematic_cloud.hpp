#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_KINEMATIC_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_KINEMATIC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicCloud
// Description
//   Cloud class to introduce kinematic parcels
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "basic_kinematic_parcel.hpp"
namespace mousse
{
  typedef KinematicCloud<Cloud<basicKinematicParcel>> basicKinematicCloud;
}
#endif

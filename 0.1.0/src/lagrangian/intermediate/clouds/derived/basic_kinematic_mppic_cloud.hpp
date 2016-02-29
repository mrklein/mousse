#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_KINEMATIC_MPPIC_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_DERIVED_BASIC_KINEMATIC_MPPIC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicMPPICCloud
// Description
//   Cloud class to introduce kinematic MPPIC parcels
#include "_cloud.hpp"
#include "_kinematic_cloud.hpp"
#include "_mppic_cloud.hpp"
#include "basic_kinematic_mppic_parcel.hpp"
namespace mousse
{
  typedef MPPICCloud
  <
    KinematicCloud
    <
      Cloud
      <
        basicKinematicMPPICParcel
      >
    >
  > basicKinematicMPPICCloud;
}
#endif

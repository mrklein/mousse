// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicKinematicMPPICCloud
// Description
//   Cloud class to introduce kinematic MPPIC parcels
#ifndef basic_kinematic_mppic_cloud_hpp_
#define basic_kinematic_mppic_cloud_hpp_
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

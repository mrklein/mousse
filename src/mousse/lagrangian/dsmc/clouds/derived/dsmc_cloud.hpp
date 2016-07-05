#ifndef LAGRANGIAN_DSMC_CLOUDS_DERIVED_DSMC_CLOUD_HPP_
#define LAGRANGIAN_DSMC_CLOUDS_DERIVED_DSMC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dsmcCloud
// Description
//   Cloud class to simulate dsmc parcels

#include "_dsmc_cloud.hpp"
#include "dsmc_parcel.hpp"


namespace mousse {

typedef DSMCCloud<dsmcParcel> dsmcCloud;

}

#endif


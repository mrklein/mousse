// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dsmcCloud
// Description
//   Cloud class to simulate dsmc parcels
// SourceFiles
//   dsmc_cloud.cpp
#ifndef dsmc_cloud_hpp_
#define dsmc_cloud_hpp_
#include "_dsmc_cloud.hpp"
#include "dsmc_parcel.hpp"
namespace mousse
{
  typedef DSMCCloud<dsmcParcel> dsmcCloud;
}
#endif

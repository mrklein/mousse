// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_parcel.hpp"
#include "_dsmc_parcel.hpp"
#include "_dsmc_cloud.hpp"
namespace mousse
{
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(DSMCParcel<particle>, 0);
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<dsmcParcel>, 0);
}

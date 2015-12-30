// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_parcel.hpp"
#include "_dsmc_parcel.hpp"
#include "_dsmc_cloud.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug(DSMCParcel<particle>, 0);
  defineTemplateTypeNameAndDebug(Cloud<dsmcParcel>, 0);
}

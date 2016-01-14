// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_parcel.hpp"
#include "_dsmc_cloud.hpp"
#include "_free_stream.hpp"
#include "_no_inflow.hpp"
namespace mousse
{
typedef DSMCCloud<dsmcParcel> CloudType;
MAKE_INFLOW_BOUNDARY_MODEL(CloudType);
// Add instances of inflow boundary model to the table
MAKE_INFLOW_BOUNDARY_MODEL_TYPE(FreeStream, CloudType);
MAKE_INFLOW_BOUNDARY_MODEL_TYPE(NoInflow, CloudType);
}

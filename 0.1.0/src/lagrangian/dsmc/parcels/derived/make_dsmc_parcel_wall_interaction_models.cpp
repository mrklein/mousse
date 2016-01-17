// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_parcel.hpp"
#include "_dsmc_cloud.hpp"
#include "_maxwellian_thermal.hpp"
#include "_specular_reflection.hpp"
#include "_mixed_diffuse_specular.hpp"

namespace mousse
{
typedef DSMCCloud<dsmcParcel> CloudType;
MAKE_WALL_INTERACTION_MODEL(CloudType);
// Add instances of wall interaction model to the table
MAKE_WALL_INTERACTION_MODEL_TYPE(MaxwellianThermal, CloudType);
MAKE_WALL_INTERACTION_MODEL_TYPE(SpecularReflection, CloudType);
MAKE_WALL_INTERACTION_MODEL_TYPE(MixedDiffuseSpecular, CloudType);
}

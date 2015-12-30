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
  makeWallInteractionModel(CloudType);
  // Add instances of wall interaction model to the table
  makeWallInteractionModelType(MaxwellianThermal, CloudType);
  makeWallInteractionModelType(SpecularReflection, CloudType);
  makeWallInteractionModelType(MixedDiffuseSpecular, CloudType);
}

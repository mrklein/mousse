// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_parcel.hpp"
#include "_dsmc_cloud.hpp"
#include "_no_binary_collision.hpp"
#include "_variable_hard_sphere.hpp"
#include "_larsen_borgnakke_variable_hard_sphere.hpp"
namespace mousse
{
  typedef DSMCCloud<dsmcParcel> CloudType;
  makeBinaryCollisionModel(DSMCCloud<dsmcParcel>);
  // Add instances of collision model to the table
  makeBinaryCollisionModelType(NoBinaryCollision, CloudType);
  makeBinaryCollisionModelType(VariableHardSphere, CloudType);
  makeBinaryCollisionModelType(LarsenBorgnakkeVariableHardSphere, CloudType);
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_particle_cloud.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<indexedParticle>, 0);
}
// Constructors 
mousse::indexedParticleCloud::indexedParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  bool readFields
)
:
  Cloud<indexedParticle>(mesh, cloudName, false)
{
  if (readFields)
  {
    indexedParticle::readFields(*this);
  }
}

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "passive_particle_cloud.hpp"


// Static Data Members

namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<passiveParticle>, 0);

}


// Constructors 
mousse::passiveParticleCloud::passiveParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  bool readFields
)
:
  Cloud<passiveParticle>{mesh, cloudName, false}
{
  if (readFields) {
    passiveParticle::readFields(*this);
  }
}


mousse::passiveParticleCloud::passiveParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  const IDLList<passiveParticle>& particles
)
:
  Cloud<passiveParticle>{mesh, cloudName, particles}
{}


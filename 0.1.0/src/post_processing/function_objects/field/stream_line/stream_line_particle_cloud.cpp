// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stream_line_particle_cloud.hpp"


// Static Data Members
namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<streamLineParticle>, 0);

}


// Constructors 
mousse::streamLineParticleCloud::streamLineParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  bool readFields
)
:
  Cloud<streamLineParticle>{mesh, cloudName, false}
{
  if (readFields) {
    streamLineParticle::readFields(*this);
  }
}


mousse::streamLineParticleCloud::streamLineParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  const IDLList<streamLineParticle>& particles
)
:
  Cloud<streamLineParticle>{mesh, cloudName, particles}
{}


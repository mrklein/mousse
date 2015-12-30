// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_bounded_stream_line_particle_cloud.hpp"
// Static Data Members
namespace mousse
{
  defineTemplateTypeNameAndDebug(Cloud<wallBoundedStreamLineParticle>, 0);
}
// Constructors 
mousse::wallBoundedStreamLineParticleCloud::wallBoundedStreamLineParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  bool readFields
)
:
  Cloud<wallBoundedStreamLineParticle>(mesh, cloudName, false)
{
  if (readFields)
  {
    wallBoundedStreamLineParticle::readFields(*this);
  }
}
mousse::wallBoundedStreamLineParticleCloud::wallBoundedStreamLineParticleCloud
(
  const polyMesh& mesh,
  const word& cloudName,
  const IDLList<wallBoundedStreamLineParticle>& particles
)
:
  Cloud<wallBoundedStreamLineParticle>(mesh, cloudName, particles)
{}

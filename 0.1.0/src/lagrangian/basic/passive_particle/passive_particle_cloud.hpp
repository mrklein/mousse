#ifndef LAGRANGIAN_BASIC_PASSIVE_PARTICLE_PASSIVE_PARTICLE_CLOUD_HPP_
#define LAGRANGIAN_BASIC_PASSIVE_PARTICLE_PASSIVE_PARTICLE_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::passiveParticleCloud
// Description
//   A Cloud of passive particles

#include "_cloud.hpp"
#include "passive_particle.hpp"


namespace mousse {

class passiveParticleCloud
:
  public Cloud<passiveParticle>
{
public:
  // Constructors
    //- Construct given mesh
    passiveParticleCloud
    (
      const polyMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
    //- Construct from mesh, cloud name, and a list of particles
    passiveParticleCloud
    (
      const polyMesh& mesh,
      const word& cloudName,
      const IDLList<passiveParticle>& particles
    );
    //- Disallow default bitwise copy construct
    passiveParticleCloud(const passiveParticleCloud&) = delete;
    //- Disallow default bitwise assignment
    passiveParticleCloud& operator=(const passiveParticleCloud&) = delete;
};
}  // namespace mousse
#endif

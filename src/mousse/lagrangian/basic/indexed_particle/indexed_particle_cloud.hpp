#ifndef LAGRANGIAN_BASIC_INDEXED_PARTICLE_INDEXED_PARTICLE_CLOUD_HPP_
#define LAGRANGIAN_BASIC_INDEXED_PARTICLE_INDEXED_PARTICLE_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedParticleCloud
// Description
//   A Cloud of particles carrying an additional index.

#include "_cloud.hpp"
#include "indexed_particle.hpp"


namespace mousse {

class indexedParticleCloud
:
  public Cloud<indexedParticle>
{
public:
  // Constructors
    //- Construct given mesh
    indexedParticleCloud
    (
      const polyMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    indexedParticleCloud(const indexedParticleCloud&) = delete;
    //- Disallow default bitwise assignment
    indexedParticleCloud& operator=(const indexedParticleCloud&) = delete;
};
}  // namespace mousse
#endif

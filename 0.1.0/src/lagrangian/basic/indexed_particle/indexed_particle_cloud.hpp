// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedParticleCloud
// Description
//   A Cloud of particles carrying an additional index.
// SourceFiles
//   indexed_particle_cloud.cpp
#ifndef indexed_particle_cloud_hpp_
#define indexed_particle_cloud_hpp_
#include "_cloud.hpp"
#include "indexed_particle.hpp"
namespace mousse
{
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

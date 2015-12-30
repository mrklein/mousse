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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    indexedParticleCloud(const indexedParticleCloud&);
    //- Disallow default bitwise assignment
    void operator=(const indexedParticleCloud&);
public:
  // Constructors
    //- Construct given mesh
    indexedParticleCloud
    (
      const polyMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
};
}  // namespace mousse
#endif

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::passiveParticleCloud
// Description
//   A Cloud of passive particles
// SourceFiles
//   passive_particle_cloud.cpp
#ifndef passive_particle_cloud_hpp_
#define passive_particle_cloud_hpp_
#include "_cloud.hpp"
#include "passive_particle.hpp"
namespace mousse
{
class passiveParticleCloud
:
  public Cloud<passiveParticle>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    passiveParticleCloud(const passiveParticleCloud&);
    //- Disallow default bitwise assignment
    void operator=(const passiveParticleCloud&);
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
};
}  // namespace mousse
#endif

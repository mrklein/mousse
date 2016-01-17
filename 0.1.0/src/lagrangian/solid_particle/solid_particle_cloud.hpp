// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidParticleCloud
// Description
//   A Cloud of solid particles
// SourceFiles
//   solid_particle_cloud.cpp
//   solid_particle_cloud_io.cpp
#ifndef solid_particle_cloud_hpp_
#define solid_particle_cloud_hpp_
#include "cloud.hpp"
#include "solid_particle.hpp"
#include "iodictionary.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class solidParticleCloud
:
  public Cloud<solidParticle>
{
  // Private data
    const fvMesh& mesh_;
    IOdictionary particleProperties_;
    scalar rhop_;
    scalar e_;
    scalar mu_;
public:
  // Constructors
    //- Construct given mesh
    solidParticleCloud
    (
      const fvMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    solidParticleCloud(const solidParticleCloud&) = delete;
    //- Disallow default bitwise assignment
    solidParticleCloud& operator=(const solidParticleCloud&) = delete;
  // Member Functions
    // Access
      virtual bool hasWallImpactDistance() const;
      inline const fvMesh& mesh() const;
      inline scalar rhop() const;
      inline scalar e() const;
      inline scalar mu() const;
    // Edit
      //- Move the particles under the influence of the given
      //  gravitational acceleration
      void move(const dimensionedVector& g);
};
}  // namespace mousse

// Member Functions 
inline const mousse::fvMesh& mousse::solidParticleCloud::mesh() const
{
  return mesh_;
}
inline mousse::scalar mousse::solidParticleCloud::rhop() const
{
  return rhop_;
}
inline mousse::scalar mousse::solidParticleCloud::e() const
{
  return e_;
}
inline mousse::scalar mousse::solidParticleCloud::mu() const
{
  return mu_;
}
#endif

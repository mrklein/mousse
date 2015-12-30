// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidParticleCloud
// Description
//   A Cloud of solid particles
// SourceFiles
//   solid_particle_cloud_i.hpp
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    solidParticleCloud(const solidParticleCloud&);
    //- Disallow default bitwise assignment
    void operator=(const solidParticleCloud&);
public:
  // Constructors
    //- Construct given mesh
    solidParticleCloud
    (
      const fvMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
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
#include "solid_particle_cloud_i.hpp"
#endif

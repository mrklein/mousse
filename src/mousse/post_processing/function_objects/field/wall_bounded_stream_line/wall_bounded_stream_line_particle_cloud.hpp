#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_WALL_BOUNDED_STREAM_LINE_WALL_BOUNDED_STREAM_LINE_PARTICLE_CLOUD_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_WALL_BOUNDED_STREAM_LINE_WALL_BOUNDED_STREAM_LINE_PARTICLE_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallBoundedStreamLineParticleCloud
// Description
//   A Cloud of streamLine particles

#include "cloud.hpp"
#include "wall_bounded_stream_line_particle.hpp"


namespace mousse {

class wallBoundedStreamLineParticleCloud
:
  public Cloud<wallBoundedStreamLineParticle>
{
public:
  //- Type of parcel the cloud was instantiated for
  typedef wallBoundedStreamLineParticle parcelType;
  // Constructors
    //- Construct given mesh
    wallBoundedStreamLineParticleCloud
    (
      const polyMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
    //- Construct from mesh, cloud name, and a list of particles
    wallBoundedStreamLineParticleCloud
    (
      const polyMesh& mesh,
      const word& cloudName,
      const IDLList<wallBoundedStreamLineParticle>& particles
    );
    //- Disallow default bitwise copy construct
    wallBoundedStreamLineParticleCloud
    (
      const wallBoundedStreamLineParticleCloud&
    ) = delete;
    //- Disallow default bitwise assignment
    wallBoundedStreamLineParticleCloud& operator=
    (
      const wallBoundedStreamLineParticleCloud&
    ) = delete;
};

}  // namespace mousse

#endif  // POST_PROCESSING_FUNCTION_OBJECTS_FIELD_WALL_BOUNDED_STREAM_LINE_WALL_BOUNDED_STREAM_LINE_PARTICLE_CLOUD_HPP_


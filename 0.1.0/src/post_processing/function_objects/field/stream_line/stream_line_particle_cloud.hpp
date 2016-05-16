#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_CLOUD_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::streamLineParticleCloud
// Description
//   A Cloud of streamLine particles

#include "cloud.hpp"
#include "stream_line_particle.hpp"


namespace mousse {

class streamLineParticleCloud
:
  public Cloud<streamLineParticle>
{
public:
  //- Type of parcel the cloud was instantiated for
  typedef streamLineParticle parcelType;
  // Constructors
    //- Construct given mesh
    streamLineParticleCloud
    (
      const polyMesh&,
      const word& cloudName = "defaultCloud",
      bool readFields = true
    );
    //- Construct from mesh, cloud name, and a list of particles
    streamLineParticleCloud
    (
      const polyMesh& mesh,
      const word& cloudName,
      const IDLList<streamLineParticle>& particles
    );
    //- Disallow default bitwise copy construct
    streamLineParticleCloud(const streamLineParticleCloud&) = delete;
    //- Disallow default bitwise assignment
    streamLineParticleCloud& operator=(const streamLineParticleCloud&) = delete;
};

}  // namespace mousse

#endif


#ifndef SAMPLING_SAMPLED_SURFACE_WRITERS_PROXY_SURFACE_WRITER_HPP_
#define SAMPLING_SAMPLED_SURFACE_WRITERS_PROXY_SURFACE_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::proxySurfaceWriter
// Description
//   A surfaceWriter that writes the geometry via the MeshedSurfaceProxy, but
//   which does not support any fields.

#include "surface_writer.hpp"


namespace mousse {

class proxySurfaceWriter
:
  public surfaceWriter
{
  // Private data
    //- The associated file extension
    word ext_;
public:
  //- Runtime type information
  TYPE_NAME("proxy");
  // Constructors
    //- Construct for a given extension
    proxySurfaceWriter(const word& ext);
  //- Destructor
  virtual ~proxySurfaceWriter();
  // Member Functions
    //- True if the surface format supports geometry in a separate file.
    //  False if geometry and field must be in a single file
    virtual bool separateGeometry()
    {
      return true;
    }
    //- Write single surface geometry to file.
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const bool verbose = false
    ) const;
};

}  // namespace mousse

#endif


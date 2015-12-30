// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::OFFsurfaceFormat
// Description
//   Provide a means of reading/writing Geomview OFF polyList format.
// See Also
//   The <a href="http://www.geoview.org">Geoview</a>
//   file format information:
//   http://www.geomview.org/docs/html/OFF.html#OFF
// Note
//   When reading, the optional \a colorspec is ignored.
//   When writing, it is set to the zone number (integer).
// SourceFiles
//   off_surface_format.cpp
#ifndef off_surface_format_hpp_
#define off_surface_format_hpp_
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class OFFsurfaceFormat
:
  public MeshedSurface<Face>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    OFFsurfaceFormat(const OFFsurfaceFormat&);
    //- Disallow default bitwise assignment
    void operator=(const OFFsurfaceFormat&);
public:
  // Constructors
    //- Construct from file name
    OFFsurfaceFormat(const fileName&);
  // Selectors
    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      (
        new OFFsurfaceFormat(name)
      );
    }
  //- Destructor
  virtual ~OFFsurfaceFormat()
  {}
  // Member Functions
    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);
    //- Read from file
    virtual bool read(const fileName&);
    //- Write object
    virtual void write(const fileName& name) const
    {
      write(name, MeshedSurfaceProxy<Face>(*this));
    }
};
}  // namespace fileFormats
}  // namespace mousse
#ifdef NoRepository
#   include "off_surface_format.cpp"
#endif
#endif

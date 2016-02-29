#ifndef SURF_MESH_SURFACE_FORMATS_AC3D_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_AC3D_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::AC3DsurfaceFormat
// Description
//   Provide a means of reading/writing AC3D format.
//   http://www.inivis.com/ac3d/man/ac3dfileformat.html
// Note
//   On input, the faces are already organized as zones.
//   The output is always sorted by zones.
//   In the absence of zones, a single zone will be assigned.
// SourceFiles
//   ac3d_surface_format.cpp


#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "ac3d_surface_format_core.hpp"

namespace mousse
{

namespace fileFormats
{

template<class Face>
class AC3DsurfaceFormat
:
  public MeshedSurface<Face>,
  public AC3DsurfaceFormatCore
{
public:
  // Constructors
    //- Construct from file name
    AC3DsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    AC3DsurfaceFormat(const AC3DsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    AC3DsurfaceFormat<Face>& operator=(const AC3DsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face>> New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face>>
      {
        new AC3DsurfaceFormat<Face>(name)
      };
    }

  //- Destructor
  virtual ~AC3DsurfaceFormat()
  {}

  // Member Functions

    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

    //- Write UnsortedMeshedSurface, the output is always sorted by zones.
    static void write(const fileName&, const UnsortedMeshedSurface<Face>&);

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
#   include "ac3d_surface_format.cpp"
#endif
#endif

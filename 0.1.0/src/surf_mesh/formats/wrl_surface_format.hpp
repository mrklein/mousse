#ifndef SURF_MESH_SURFACE_FORMATS_WRL_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_WRL_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::WRLsurfaceFormat
// Description
//   Provide a means of writing VRML97 (wrl) format.

#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "wrl_surface_format_core.hpp"


namespace mousse {
namespace fileFormats {

template<class Face>
class WRLsurfaceFormat
:
  public MeshedSurface<Face>,
  public WRLsurfaceFormatCore
{
public:
  // Constructors

    //- Construct null
    WRLsurfaceFormat();

    //- Disallow default bitwise copy construct
    WRLsurfaceFormat(const WRLsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    WRLsurfaceFormat<Face>& operator=(const WRLsurfaceFormat<Face>&) = delete;

  //- Destructor
  virtual ~WRLsurfaceFormat()
  {}

  // Member Functions

    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

    //- Write object file
    virtual void write(const fileName& name) const
    {
      write(name, MeshedSurfaceProxy<Face>(*this));
    }

};

}  // namespace fileFormats
}  // namespace mousse

#include "wrl_surface_format.ipp"

#endif

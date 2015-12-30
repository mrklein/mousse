// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::WRLsurfaceFormat
// Description
//   Provide a means of writing VRML97 (wrl) format.
// SourceFiles
//   wrl_surface_format.cpp
#ifndef wrl_surface_format_hpp_
#define wrl_surface_format_hpp_
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "wrl_surface_format_core.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class WRLsurfaceFormat
:
  public MeshedSurface<Face>,
  public WRLsurfaceFormatCore
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    WRLsurfaceFormat(const WRLsurfaceFormat<Face>&);
    //- Disallow default bitwise assignment
    void operator=(const WRLsurfaceFormat<Face>&);
public:
  // Constructors
    //- Construct null
    WRLsurfaceFormat();
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
#ifdef NoRepository
#   include "wrl_surface_format.cpp"
#endif
#endif

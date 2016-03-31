#ifndef SURF_MESH_SURFACE_FORMATS_VTK_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_VTK_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::VTKsurfaceFormat
// Description
//   Provide a means of reading/writing VTK legacy format.
//   The output is never sorted by zone.

#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "vtk_surface_format_core.hpp"


namespace mousse {
namespace fileFormats {

template<class Face>
class VTKsurfaceFormat
:
  public MeshedSurface<Face>,
  public VTKsurfaceFormatCore
{
  // Private Member Functions

    //- Write header information about number of polygon points
    static void writeHeaderPolygons(Ostream&, const UList<Face>&);

public:
  // Constructors

    //- Construct from file name
    VTKsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    VTKsurfaceFormat(const VTKsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    VTKsurfaceFormat<Face>& operator=(const VTKsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face>> New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face>>
      {
        new VTKsurfaceFormat<Face>{name}
      };
    }

  //- Destructor
  virtual ~VTKsurfaceFormat()
  {}

  // Member Functions

    // Write
    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

    //- Write UnsortedMeshedSurface, the output remains unsorted
    static void write(const fileName&, const UnsortedMeshedSurface<Face>&);

    //- Read from file
    virtual bool read(const fileName&);

    //- Write object file
    virtual void write(const fileName& name) const
    {
      write(name, MeshedSurfaceProxy<Face>(*this));
    }
};

}  // namespace fileFormats
}  // namespace mousse

#include "vtk_surface_format.ipp"

#endif

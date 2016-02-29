#ifndef SURF_MESH_SURFACE_FORMATS_OFS_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_OFS_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::OFSsurfaceFormat
// Description
//   Provide a means of reading/writing the single-file OpenFOAM surface format.
// Note
//   This class provides more methods than the regular surface format interface.
// SourceFiles
//   ofs_surface_format.cpp


#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "ofs_surface_format_core.hpp"

namespace mousse
{

namespace fileFormats
{

template<class Face>
class OFSsurfaceFormat
:
  public MeshedSurface<Face>,
  public OFSsurfaceFormatCore
{
public:

  // Constructors

    //- Construct from file name
    OFSsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    OFSsurfaceFormat(const OFSsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    OFSsurfaceFormat<Face>& operator=(const OFSsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      {
        new OFSsurfaceFormat<Face>{name}
      };
    }

  //- Destructor
  virtual ~OFSsurfaceFormat()
  {}

  // Member Functions

    //- Read surface mesh components
    static bool read
    (
      Istream&,
      pointField&,
      List<Face>&,
      List<surfZone>&
    );

    //- Read MeshedSurface
    static bool read
    (
      Istream&,
      MeshedSurface<Face>&
    );

    //- Read UnsortedMeshedSurface
    //  The output is sorted by zones
    static bool read
    (
      Istream&,
      UnsortedMeshedSurface<Face>&
    );

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
#   include "ofs_surface_format.cpp"
#endif
#endif

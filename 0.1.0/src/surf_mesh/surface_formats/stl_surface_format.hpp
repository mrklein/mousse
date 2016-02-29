#ifndef SURF_MESH_SURFACE_FORMATS_STL_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_STL_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STLsurfaceFormat
// Description
//   Provide a means of reading/writing STL files (ASCII and binary).
// Note
//   For efficiency, the zones are sorted before creating the faces.
//   The class is thus derived from MeshedSurface.
// SourceFiles
//   stl_surface_format.cpp
//   stl_surface_format_ascii.l


#include "stl_surface_format_core.hpp"
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"

namespace mousse
{

namespace fileFormats
{

template<class Face>
class STLsurfaceFormat
:
  public MeshedSurface<Face>
{
  // Private Member Functions

    //- Write Face (ASCII)
    static inline void writeShell
    (
      Ostream&,
      const pointField&,
      const Face&
    );

    //- Write Face (BINARY)
    static inline void writeShell
    (
      ostream&,
      const pointField&,
      const Face&,
      const label zoneI
    );

public:

  // Constructors

    //- Construct from file name
    STLsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    STLsurfaceFormat(const STLsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    STLsurfaceFormat<Face>& operator=(const STLsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      {
        new STLsurfaceFormat<Face>{name}
      };
    }

  //- Destructor
  virtual ~STLsurfaceFormat()
  {}

  // Member Functions

    //- Write surface mesh components by proxy (as ASCII)
    static void writeAscii
    (
      const fileName&,
      const MeshedSurfaceProxy<Face>&
    );

    //- Write surface mesh components by proxy (as BINARY)
    static void writeBinary
    (
      const fileName&,
      const MeshedSurfaceProxy<Face>&
    );

    //- Write surface mesh components by proxy
    //  as ASCII or BINARY, depending on the extension
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

    //- Write UnsortedMeshedSurface (as ASCII) sorted by zone
    static void writeAscii
    (
      const fileName&,
      const UnsortedMeshedSurface<Face>&
    );

    //- Write UnsortedMeshedSurface (as BINARY) unsorted by zone
    static void writeBinary
    (
      const fileName&,
      const UnsortedMeshedSurface<Face>&
    );

    //- Write UnsortedMeshedSurface
    //  as ASCII or BINARY, depending on the extension
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
#   include "stl_surface_format.cpp"
#endif
#endif

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::TRIsurfaceFormat
// Description
//   Provide a means of reading/writing .tri format.
// Note
//   For efficiency, the zones are sorted before creating the faces.
//   The class is thus derived from MeshedSurface.
// SourceFiles
//   tri_surface_format.cpp
#ifndef tri_surface_format_hpp_
#define tri_surface_format_hpp_
#include "tri_surface_format_core.hpp"
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class TRIsurfaceFormat
:
  public MeshedSurface<Face>
{
  // Private Member Functions
    static inline void writeShell
    (
      Ostream&,
      const pointField&,
      const Face&,
      const label zoneI
    );
public:
  // Constructors
    //- Construct from file name
    TRIsurfaceFormat(const fileName&);
  // Selectors
    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      (
        new TRIsurfaceFormat<Face>(name)
      );
    }
    //- Disallow default bitwise copy construct
    TRIsurfaceFormat(const TRIsurfaceFormat<Face>&) = delete;
    //- Disallow default bitwise assignment
    TRIsurfaceFormat<Face>& operator=(const TRIsurfaceFormat<Face>&) = delete;
  //- Destructor
  virtual ~TRIsurfaceFormat()
  {}
  // Member Functions
    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);
    //- Write UnsortedMeshedSurface,
    //  by default the output is not sorted by zones
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
#   include "tri_surface_format.cpp"
#endif
#endif

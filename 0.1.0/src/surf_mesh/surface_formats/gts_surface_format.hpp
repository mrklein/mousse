// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::GTSsurfaceFormat
// Description
//   Provide a means of reading/writing GTS format.
//   The output is never sorted by zone and is only written if it consists
//   entirely of triangles.
// SourceFiles
//   gts_surface_format.cpp
#ifndef gts_surface_format_hpp_
#define gts_surface_format_hpp_
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class GTSsurfaceFormat
:
  public UnsortedMeshedSurface<Face>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    GTSsurfaceFormat(const GTSsurfaceFormat<Face>&);
    //- Disallow default bitwise assignment
    void operator=(const GTSsurfaceFormat<Face>&);
public:
  // Constructors
    //- Construct from file name
    GTSsurfaceFormat(const fileName&);
  // Selectors
    //- Read file and return surface
    static autoPtr<UnsortedMeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<UnsortedMeshedSurface<Face> >
      (
        new GTSsurfaceFormat<Face>(name)
      );
    }
  //- Destructor
  virtual ~GTSsurfaceFormat()
  {}
  // Member Functions
    //- Write MeshedSurface
    static void write(const fileName&, const MeshedSurface<Face>&);
    //- Write UnsortedMeshedSurface, the output remains unsorted
    static void write(const fileName&, const UnsortedMeshedSurface<Face>&);
    //- Read from file
    virtual bool read(const fileName&);
    //- Write object
    virtual void write(const fileName& name) const
    {
      write(name, *this);
    }
};
}  // namespace fileFormats
}  // namespace mousse
#ifdef NoRepository
#   include "gts_surface_format.cpp"
#endif
#endif

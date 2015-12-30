// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::OBJsurfaceFormat
// Description
//   Provide a means of reading/writing Alias/Wavefront OBJ format.
//   Does not handle negative face indices.
// SourceFiles
//   obj_surface_format.cpp
#ifndef obj_surface_format_hpp_
#define obj_surface_format_hpp_
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class OBJsurfaceFormat
:
  public MeshedSurface<Face>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    OBJsurfaceFormat(const OBJsurfaceFormat<Face>&);
    //- Disallow default bitwise assignment
    void operator=(const OBJsurfaceFormat<Face>&);
public:
  // Constructors
    //- Construct from file name
    OBJsurfaceFormat(const fileName&);
  // Selectors
    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      (
        new OBJsurfaceFormat<Face>(name)
      );
    }
  //- Destructor
  virtual ~OBJsurfaceFormat()
  {}
  // Member Functions
    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);
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
#ifdef NoRepository
#   include "obj_surface_format.cpp"
#endif
#endif

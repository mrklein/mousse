// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::SMESHsurfaceFormat
// Description
//   Provide a means of writing tetgen SMESH format.
//   Tetgen http://tetgen.berlios.de
// See Also
//   File format information:
//   http://tetgen.berlios.de/fformats.smesh.html
// SourceFiles
//   smesh_surface_format.cpp
#ifndef smesh_surface_format_hpp_
#define smesh_surface_format_hpp_
#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
template<class Face>
class SMESHsurfaceFormat
:
  public MeshedSurface<Face>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    SMESHsurfaceFormat(const SMESHsurfaceFormat<Face>&);
    //- Disallow default bitwise assignment
    void operator=(const SMESHsurfaceFormat<Face>&);
public:
  // Constructors
    //- Construct null
    SMESHsurfaceFormat();
  //- Destructor
  virtual ~SMESHsurfaceFormat()
  {}
  // Member Functions
    //- Write surface mesh components by proxy
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);
    //- Write object
    virtual void write(const fileName& name) const
    {
      write(name, MeshedSurfaceProxy<Face>(*this));
    }
};
}  // namespace fileFormats
}  // namespace mousse
#ifdef NoRepository
#   include "smesh_surface_format.cpp"
#endif
#endif

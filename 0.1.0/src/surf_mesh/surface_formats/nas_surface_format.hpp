// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::NASsurfaceFormat
// Description
//   Nastran surface reader.
//   - Uses the Ansa "$ANSA_NAME" or the Hypermesh "$HMNAME COMP" extensions
//    to obtain zone names.
//   - Handles Nastran short and long formats, but not free format.
//   - Properly handles the Nastran compact floating point notation: \n
//   \verbatim
//     GRID          28        10.20269-.030265-2.358-8
//   \endverbatim
// SourceFiles
//   nas_surface_format.cpp

#ifndef nas_surface_format_hpp_
#define nas_surface_format_hpp_

#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "nas_core.hpp"

namespace mousse
{

namespace fileFormats
{

template<class Face>
class NASsurfaceFormat
:
  public MeshedSurface<Face>,
  public NASCore
{
public:
  // Constructors
    //- Construct from file name
    NASsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    NASsurfaceFormat(const NASsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    NASsurfaceFormat<Face>& operator=(const NASsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face>> New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face>>
      {
        new NASsurfaceFormat<Face>{name}
      };
    }

  //- Destructor
  virtual ~NASsurfaceFormat()
  {}

  // Member Functions

    //- Read from a file
    virtual bool read(const fileName&);

};

}  // namespace fileFormats

}  // namespace mousse
#ifdef NoRepository
#   include "nas_surface_format.cpp"
#endif
#endif

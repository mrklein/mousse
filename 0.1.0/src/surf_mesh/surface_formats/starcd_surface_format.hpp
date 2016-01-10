// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STARCDsurfaceFormat
// Description
//   Read/write the surface shells from pro-STAR vrt/cel files.
// Note
//   Uses the extension \a .inp (input) to denote the format.
// See Also
//   mousse::meshReaders::STARCD
// SourceFiles
//   starcd_surface_format.cpp

#ifndef starcd_surface_format_hpp_
#define starcd_surface_format_hpp_

#include "meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "unsorted_meshed_surface.hpp"
#include "starcd_surface_format_core.hpp"

namespace mousse
{

namespace fileFormats
{

template<class Face>
class STARCDsurfaceFormat
:
  public MeshedSurface<Face>,
  public STARCDsurfaceFormatCore
{
  // Private Data

    //- STAR-CD identifier for shell shapes (2d elements)
    static const int starcdShellShape_ = 3;

    //- STAR-CD identifier for shell type (shells vs. baffles)
    static const int starcdShellType_  = 4;

  // Private Member Functions

    static inline void writeShell
    (
      Ostream&,
      const Face&,
      const label cellId,
      const label cellTableId
    );
public:
  // Constructors

    //- Construct from file name
    STARCDsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    STARCDsurfaceFormat(const STARCDsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    STARCDsurfaceFormat<Face>& operator=
    (
      const STARCDsurfaceFormat<Face>&
    ) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<MeshedSurface<Face> > New(const fileName& name)
    {
      return autoPtr<MeshedSurface<Face> >
      {
        new STARCDsurfaceFormat<Face>{name}
      };
    }

  //- Destructor
  virtual ~STARCDsurfaceFormat()
  {}

  // Member Functions

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
#   include "starcd_surface_format.cpp"
#endif
#endif

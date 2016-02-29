#ifndef SURF_MESH_SURFACE_FORMATS_FTR_SURFACE_FORMAT_HPP_
#define SURF_MESH_SURFACE_FORMATS_FTR_SURFACE_FORMAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::FTRsurfaceFormat
// Description
//   Reading of the (now deprecated and infrequently used)
//   Foam Trisurface Format.
// Deprecated
//   Other formats are better. (deprecated Mar 2009)
// SourceFiles
//   ftr_surface_format.cpp


#include "ifstream.hpp"
#include "meshed_surface.hpp"
#include "unsorted_meshed_surface.hpp"

namespace mousse
{
namespace fileFormats
{
template<class Face>
class FTRsurfaceFormat
:
  public UnsortedMeshedSurface<Face>
{
  // Private classes

    //- Read compatibility for ftr patch definitions
    class ftrPatch
    {
      //- Name of patch
      word name_;
      //- Type of patch (ignored since it is usually "empty")
      word type_;
    public:
      const word& name() const
      {
        return name_;
      }
      friend Istream& operator>>(Istream& is, ftrPatch& p)
      {
        is >> p.name_ >> p.type_;
        return is;
      }
    };

public:
  // Constructors
    //- Construct from file name
    FTRsurfaceFormat(const fileName&);

    //- Disallow default bitwise copy construct
    FTRsurfaceFormat(const FTRsurfaceFormat<Face>&) = delete;

    //- Disallow default bitwise assignment
    FTRsurfaceFormat<Face>& operator=(const FTRsurfaceFormat<Face>&) = delete;

  // Selectors

    //- Read file and return surface
    static autoPtr<UnsortedMeshedSurface<Face>> New(const fileName& name)
    {
      return autoPtr<UnsortedMeshedSurface<Face>>
      {
        new FTRsurfaceFormat<Face>(name)
      };
    }

  //- Destructor
  virtual ~FTRsurfaceFormat()
  {}

  // Member Functions
    //- Read from file
    virtual bool read(const fileName&);

};

}  // namespace fileFormats

}  // namespace mousse

#ifdef NoRepository
#   include "ftr_surface_format.cpp"
#endif
#endif

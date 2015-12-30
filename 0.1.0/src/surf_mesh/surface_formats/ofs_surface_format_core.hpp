// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::OFSsurfaceFormatCore
// Description
//   Internal class used by the OFSsurfaceFormat
// SourceFiles
//   ofs_surface_format_core.cpp
#ifndef ofs_surface_format_core_hpp_
#define ofs_surface_format_core_hpp_
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
#include "unsorted_meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
class OFSsurfaceFormatCore
{
protected:
  // Protected Member Functions
    //- Write header information and surfZoneList
    static void writeHeader
    (
      Ostream&,
      const pointField&,
      const UList<surfZone>&
    );
};
}  // namespace fileFormats
}  // namespace mousse
#endif

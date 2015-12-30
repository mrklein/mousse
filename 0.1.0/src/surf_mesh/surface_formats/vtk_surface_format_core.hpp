// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::VTKsurfaceFormatCore
// Description
//   Internal class used by the VTKsurfaceFormat
// SourceFiles
//   vtk_surface_format_core.cpp
#ifndef vtk_surface_format_core_hpp_
#define vtk_surface_format_core_hpp_
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
class VTKsurfaceFormatCore
{
protected:
  // Protected Member Functions
    //- Write header information with points
    static void writeHeader
    (
      Ostream&,
      const pointField&
    );
    //- Write trailing information with zone information
    static void writeTail(Ostream&, const UList<surfZone>&);
    //- Write trailing information with zone Ids
    static void writeTail(Ostream&, const labelUList& zoneIds);
};
}  // namespace fileFormats
}  // namespace mousse
#endif

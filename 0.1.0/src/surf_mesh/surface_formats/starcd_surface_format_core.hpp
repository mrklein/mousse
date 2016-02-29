#ifndef SURF_MESH_SURFACE_FORMATS_STARCD_SURFACE_FORMAT_CORE_HPP_
#define SURF_MESH_SURFACE_FORMATS_STARCD_SURFACE_FORMAT_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STARCDsurfaceFormatCore
// Description
//   Internal class used by the STARCDsurfaceFormat
// SourceFiles
//   starcd_surface_format_core.cpp
#include "ifstream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
#include "starcd_core.hpp"
namespace mousse
{
namespace fileFormats
{
class STARCDsurfaceFormatCore
:
  public STARCDCore
{
protected:
  // Protected Member Functions
  static Map<word> readInpCellTable(IFstream&);
  static void writeCase
  (
    Ostream&,
    const pointField&,
    const label nFaces,
    const UList<surfZone>&
  );
};
}  // namespace fileFormats
}  // namespace mousse
#endif

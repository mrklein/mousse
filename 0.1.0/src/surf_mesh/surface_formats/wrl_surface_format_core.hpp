// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::WRLsurfaceFormatCore
// Description
//   Internal class used by the WRLsurfaceFormat
// SourceFiles
//   wrl_surface_format_core.cpp
#ifndef wrl_surface_format_core_hpp_
#define wrl_surface_format_core_hpp_
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
class WRLsurfaceFormatCore
{
protected:
  // Protected Member Functions
  //- Write file header
  static void writeHeader
  (
    Ostream&,
    const pointField&,
    const label nFaces,
    const UList<surfZone>&
  );
  //- Write appearance node
  static void writeAppearance(Ostream&);
};
}  // namespace fileFormats
}  // namespace mousse
#endif

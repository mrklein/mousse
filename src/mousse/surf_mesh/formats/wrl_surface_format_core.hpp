#ifndef SURF_MESH_SURFACE_FORMATS_WRL_SURFACE_FORMAT_CORE_HPP_
#define SURF_MESH_SURFACE_FORMATS_WRL_SURFACE_FORMAT_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::WRLsurfaceFormatCore
// Description
//   Internal class used by the WRLsurfaceFormat

#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"


namespace mousse {
namespace fileFormats {

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

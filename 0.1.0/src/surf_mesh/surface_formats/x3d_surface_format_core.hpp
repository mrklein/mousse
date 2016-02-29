#ifndef SURF_MESH_SURFACE_FORMATS_X3D_SURFACE_FORMAT_CORE_HPP_
#define SURF_MESH_SURFACE_FORMATS_X3D_SURFACE_FORMAT_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::X3DsurfaceFormatCore
// Description
//   Internal class used by the X3DsurfaceFormat
// SourceFiles
//   x3d_surface_format_core.cpp
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
class X3DsurfaceFormatCore
{
protected:
  // Protected Member Functions
  //- Write file header
  static void writeHeader(Ostream&);
  //- Write appearance node
  static void writeAppearance(Ostream&);
};
}  // namespace fileFormats
}  // namespace mousse
#endif

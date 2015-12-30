// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "off_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read MeshedSurface
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  OFFsurfaceFormat,
  face,
  fileExtension,
  off
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  OFFsurfaceFormat,
  triFace,
  fileExtension,
  off
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OFFsurfaceFormat,
  face,
  write,
  fileExtension,
  off
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OFFsurfaceFormat,
  triFace,
  write,
  fileExtension,
  off
);
}
}

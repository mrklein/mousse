// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "starcd_surface_format.hpp"
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
  STARCDsurfaceFormat,
  face,
  fileExtension,
  inp
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  STARCDsurfaceFormat,
  triFace,
  fileExtension,
  inp
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  STARCDsurfaceFormat,
  face,
  write,
  fileExtension,
  inp
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  STARCDsurfaceFormat,
  triFace,
  write,
  fileExtension,
  inp
);
}
}

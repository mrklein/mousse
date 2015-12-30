// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_surface_format.hpp"
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
  OBJsurfaceFormat,
  face,
  fileExtension,
  obj
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  OBJsurfaceFormat,
  triFace,
  fileExtension,
  obj
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OBJsurfaceFormat,
  face,
  write,
  fileExtension,
  obj
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OBJsurfaceFormat,
  triFace,
  write,
  fileExtension,
  obj
);
}
}

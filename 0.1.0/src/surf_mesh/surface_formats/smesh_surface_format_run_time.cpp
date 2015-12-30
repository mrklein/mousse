// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "smesh_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  SMESHsurfaceFormat,
  face,
  write,
  fileExtension,
  smesh
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  SMESHsurfaceFormat,
  triFace,
  write,
  fileExtension,
  smesh
);
}
}

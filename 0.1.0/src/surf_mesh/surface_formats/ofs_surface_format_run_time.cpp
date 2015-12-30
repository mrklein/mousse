// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ofs_surface_format.hpp"
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
  OFSsurfaceFormat,
  face,
  fileExtension,
  ofs
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  OFSsurfaceFormat,
  triFace,
  fileExtension,
  ofs
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OFSsurfaceFormat,
  face,
  write,
  fileExtension,
  ofs
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  OFSsurfaceFormat,
  triFace,
  write,
  fileExtension,
  ofs
);
}
}

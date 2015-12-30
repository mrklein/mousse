// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_format.hpp"
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
  TRIsurfaceFormat,
  face,
  fileExtension,
  tri
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  TRIsurfaceFormat,
  triFace,
  fileExtension,
  tri
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  TRIsurfaceFormat,
  face,
  write,
  fileExtension,
  tri
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  TRIsurfaceFormat,
  triFace,
  write,
  fileExtension,
  tri
);
// write UnsortedMeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  TRIsurfaceFormat,
  face,
  write,
  fileExtension,
  tri
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  TRIsurfaceFormat,
  triFace,
  write,
  fileExtension,
  tri
);
}
}

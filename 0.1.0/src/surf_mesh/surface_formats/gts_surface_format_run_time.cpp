// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gts_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read UnsortedMeshedSurface
addNamedTemplatedToRunTimeSelectionTable
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  face,
  fileExtension,
  gts
);
addNamedTemplatedToRunTimeSelectionTable
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  triFace,
  fileExtension,
  gts
);
// write MeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurface,
  GTSsurfaceFormat,
  face,
  write,
  fileExtension,
  gts
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurface,
  GTSsurfaceFormat,
  triFace,
  write,
  fileExtension,
  gts
);
// write UnsortedMeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  face,
  write,
  fileExtension,
  gts
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  GTSsurfaceFormat,
  triFace,
  write,
  fileExtension,
  gts
);
}
}

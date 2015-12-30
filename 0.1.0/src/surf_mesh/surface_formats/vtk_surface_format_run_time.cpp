// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_surface_format.hpp"
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
  VTKsurfaceFormat,
  face,
  fileExtension,
  vtk
);
addNamedTemplatedToRunTimeSelectionTable
(
  MeshedSurface,
  VTKsurfaceFormat,
  triFace,
  fileExtension,
  vtk
);
// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  VTKsurfaceFormat,
  face,
  write,
  fileExtension,
  vtk
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  MeshedSurfaceProxy,
  VTKsurfaceFormat,
  triFace,
  write,
  fileExtension,
  vtk
);
// write UnsortedMeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  VTKsurfaceFormat,
  face,
  write,
  fileExtension,
  vtk
);
addNamedTemplatedToMemberFunctionSelectionTable
(
  UnsortedMeshedSurface,
  VTKsurfaceFormat,
  triFace,
  write,
  fileExtension,
  vtk
);
}
}

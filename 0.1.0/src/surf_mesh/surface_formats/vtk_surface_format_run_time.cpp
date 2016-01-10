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
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  VTKsurfaceFormat,
  face,
  fileExtension,
  vtk
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  VTKsurfaceFormat,
  triFace,
  fileExtension,
  vtk
);
// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  VTKsurfaceFormat,
  face,
  write,
  fileExtension,
  vtk
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  VTKsurfaceFormat,
  triFace,
  write,
  fileExtension,
  vtk
);
// write UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  VTKsurfaceFormat,
  face,
  write,
  fileExtension,
  vtk
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
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

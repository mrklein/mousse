// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stl_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fileFormats
{
// read MeshedSurface (ascii)
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  STLsurfaceFormat,
  face,
  fileExtension,
  stl
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  STLsurfaceFormat,
  triFace,
  fileExtension,
  stl
);
// read MeshedSurface (binary)
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  STLsurfaceFormat,
  face,
  fileExtension,
  stlb
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  STLsurfaceFormat,
  triFace,
  fileExtension,
  stlb
);
// write MeshedSurfaceProxy (ascii)
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  STLsurfaceFormat,
  face,
  write,
  fileExtension,
  stl
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  STLsurfaceFormat,
  triFace,
  write,
  fileExtension,
  stl
);
// write MeshedSurfaceProxy (binary)
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  STLsurfaceFormat,
  face,
  write,
  fileExtension,
  stlb
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  STLsurfaceFormat,
  triFace,
  write,
  fileExtension,
  stlb
);
// write UnsortedMeshedSurface (ascii)
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  STLsurfaceFormat,
  face,
  write,
  fileExtension,
  stl
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  STLsurfaceFormat,
  triFace,
  write,
  fileExtension,
  stl
);
// write UnsortedMeshedSurface (binary)
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  STLsurfaceFormat,
  face,
  write,
  fileExtension,
  stlb
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  STLsurfaceFormat,
  triFace,
  write,
  fileExtension,
  stlb
);
}
}

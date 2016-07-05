// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read MeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  OBJsurfaceFormat,
  face,
  fileExtension,
  obj
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  OBJsurfaceFormat,
  triFace,
  fileExtension,
  obj
);
// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  OBJsurfaceFormat,
  face,
  write,
  fileExtension,
  obj
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
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

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ac3d_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read MeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  AC3DsurfaceFormat,
  face,
  fileExtension,
  ac
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  AC3DsurfaceFormat,
  triFace,
  fileExtension,
  ac
);
// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  AC3DsurfaceFormat,
  face,
  write,
  fileExtension,
  ac
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  AC3DsurfaceFormat,
  triFace,
  write,
  fileExtension,
  ac
);
// write UnsortedMeshedSurface
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  AC3DsurfaceFormat,
  face,
  write,
  fileExtension,
  ac
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  UnsortedMeshedSurface,
  AC3DsurfaceFormat,
  triFace,
  write,
  fileExtension,
  ac
);

}
}

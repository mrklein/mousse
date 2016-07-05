// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "x3d_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  X3DsurfaceFormat,
  face,
  write,
  fileExtension,
  x3d
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  X3DsurfaceFormat,
  triFace,
  write,
  fileExtension,
  x3d
);

}
}

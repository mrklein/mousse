// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ofs_surface_format.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fileFormats {

// read MeshedSurface
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  OFSsurfaceFormat,
  face,
  fileExtension,
  ofs
);
ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE
(
  MeshedSurface,
  OFSsurfaceFormat,
  triFace,
  fileExtension,
  ofs
);
// write MeshedSurfaceProxy
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  MeshedSurfaceProxy,
  OFSsurfaceFormat,
  face,
  write,
  fileExtension,
  ofs
);
ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE
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
